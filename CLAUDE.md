# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

ToolMap is a wxWidgets desktop GIS app (C++) for digitizing data and building multi-layer GIS projects. User docs: http://toolmap.readthedocs.io. Build guide: https://terranum-ch.github.io/ToolMap-dev-doc/instructions/build.html

## Build

Dependencies come from **Conan 1.x** (`pip install conan==1.*`), not Conan 2. The custom packages (wxWidgets, MariaDB embedded, GDAL, wxPdfDocument) are hosted on GitLab:

```bash
conan remote add gitlab https://gitlab.com/api/v4/packages/conan --force
# Windows CI also runs: conan config set general.revisions_enabled=0
# Linux needs a libstdc++11 profile: conan profile update settings.compiler.libcxx=libstdc++11 default
```

Out-of-source build in a `bin/` directory (this is what CI does):

```bash
mkdir bin && cd bin
conan install .. -o unit_test=True --build=missing     # generates conanbuildinfo.cmake, copies DLLs/proj/errmsg.sys/certs
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUSE_UNITTEST=1
cmake --build . --config RelWithDebInfo
# or, on Linux/macOS: conan build ..   (configures + builds using conanfile.py's build())
```

`conan install` has to run before CMake, because `CMakeLists.txt` includes `${CMAKE_BINARY_DIR}/conanbuildinfo.cmake`. The `imports()` step in `conanfile.py` copies the runtime files the app and tests need: shared libs, MariaDB `errmsg.sys` (to `share/mysql`), PROJ data (to `share/proj`), web layer XMLs and SSL certs. If the app or tests fail at runtime because of a missing DB error file or projection data, check this step. CI also sets `PROJ_LIB=<build>/share/proj`.

CMake options: `USE_UNITTEST`, `USE_CODECOV` (GCC/lcov, target `code-coverage`), `USE_CODECOVERAGE_IDE`, `WITH_TOOLBASVIEW`. Conan options: `unit_test`, `code_coverage`.

Packaging: `cpack -C <config> -G ZIP` / `-G WIX` (Windows), configured in `build/cpack/`.

## Tests

GoogleTest. Everything builds into a single `UnitTests` executable that is written to `<build>/bin/`. Run it from that directory:

```bash
cd bin/bin && ./UnitTests
./UnitTests --gtest_filter=TestDatabaseTM.*      # single suite / test
```

- Test sources: `test/src/*.cpp` (picked up by glob). They link against the `ToolMap_lib` static library, which is every `src/**/*.cpp` except `src/core/toolmap.cpp` (the app entry point).
- Test data (sample ToolMap projects) is fetched at configure time from `terranum-ch/ToolMap-test-files`, pinned to a commit in `test/CMakeLists.txt` that matches a DB schema version. Set `-DUNIT_TESTING_PATH=<dir>` to use a local copy instead. The paths end up in the generated `test_param.h` (from `test/src/test_param.h.in`).
- `test/src/main.cpp` calls `wxInitialize()` and registers a global `DatabaseEnvironment`, which holds one shared `DataBaseTM` (the embedded MariaDB can only be initialized once per process). Tests use `DatabaseEnvironment::m_db` instead of creating their own database.

## Code style

`.clang-format`: Google-based, 4-space indent, 120 columns, left pointer alignment, access modifiers at -2. The codebase uses wxWidgets types throughout (`wxString`, `wxArrayString`, `wxDELETE`, `_T()`), and most classes have a `tm` prefix.

## Architecture

- **`src/core/toolmap.cpp/.h`**: `ToolMapApp` / `ToolMapFrame`. The frame owns and wires together the managers listed below and the AUI panels.
- **`src/database/`**: `DataBase` wraps the **embedded MariaDB** library (compiled with `-DEMBEDDED_LIBRARY`). Each ToolMap project is a MySQL database directory on disk. `DataBaseTM` (`database_tm.cpp`, very large) adds all ToolMap-specific schema and queries. `tmProjectMerge` and `tmProjectMaintenance` handle multi-project operations.
- **Schema versioning**: `TM_DATABASE_VERSION` in `src/database/database_tm.h`. `tmProjectUpdater` (`src/core/tmprojectupdater.cpp`) migrates older projects step by step (`_232to233()`, ...). To change the schema: bump the constant, add a migration step, update `resources/database/misc/changelog_database.txt`, and update the pinned test-files commit if the fixtures need the new version.
- **`src/gis/`**: GIS engine on top of GDAL/OGR.
  - `tmLayerManager` is the central controller for the TOC, the layers and rendering through `tmRenderer` and `tmDrawer`.
  - The data sources form a class hierarchy under `tmGISData`. Vector: `tmGISDataVectorMYSQL` stores the project layers inside the DB, `tmGISDataVectorSHP` handles external files. Raster: tiff, jpeg, egrid and web/WMS.
  - `tmEditManager` handles digitizing and editing.
  - `tmAttributionManager` and the `tmAttributionData*` classes assign objects/attributes to geometries.
  - Symbology: the `tmSymbol*` classes and `tmSymbolDlg*`, with rule-based styling in `tmsymbolrule`.
- **`src/gui/`**: dialogs and panels. Some were designed in wxFormBuilder (`.fbp`) or XRC (`resources/dialogs/`). Generated code is pasted into the `.cpp` files, so if you edit a dialog's code, keep its `.fbp` file in sync or note the divergence.
- **Inter-component communication** goes through custom wxWidgets events (`DECLARE_EVENT_TYPE` in headers such as `src/gis/tmmanagerevent.h`) posted to the frame or to the managers, not through direct calls. When adding a feature that spans managers, follow this pattern.
- **`src/components/`**: vendored third-party/utility code (wxTreeMultiCtrl, wxSerialize, geodesic, crash report, update checker).
- **`apps/`**: standalone side utilities and prototypes (shpimporter, toolmerge, lineunion, ...). Each has its own CMake/Conan setup and is not part of the main build.

Version numbers: major/minor are set in the root `CMakeLists.txt`, and the patch number comes from git (`build/cmake/version.cmake`). Release notes are kept in `changelog.md`.
