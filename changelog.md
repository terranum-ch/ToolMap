# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [4.0.2800 - 2024-10-29] - MORCOTE

### Changed

- Bumping GDAL to 3.8.3
- Bumping wxWidgets to 3.2.5
- Bumping proj to 9.3.1
- Updating deprecated curl code.
- Adding some unit tests.

### Fixed

- Fixing crash when moving a label point (see #502).
- Fixing issues with special characters in object kinds (see #510).


## [4.0.2734 - 2023-10-27] - ANDERMATT

### Added

- Changing multiple symbology for shape file layers has been added (see #498)

### Changed

- Bumping GDAL to 3.7.0
- Bumping wxWidgets to 3.2.2.1
- Bumping libtiff to 4.6.0
- Bumping proj to 9.3.0

### Fixed

- Fixing new issue with new projects under Windows.
- Fixing snapping to the nearest point (see #497).


## [4.0.2691 - 2022-12-20] - NUGLAR

### Added

- Adding code coverage (see #478)
- Using conan for the build system (see #466)

### Changed

- New application logo (see #486)
- Using SVG for toolbar's icon instead of bitmaps (see #487)
- Complete redesign of the table of contents to allow layers grouping (see #477, #178, #467)
- Improving the crash report email, adding the Toolmap's version (see #483)
- Moving from MySQL 5.6.51 to MariaDB 10.6.10 (see #471)
- Bumping GDAL to 3.5.2
- Bumping wxWidgets to 3.2.1
- Moving to gtest for unit testing (see #472)
- Moving ToolbasView to a separate project

### Fixed

- Symbology isn't duplicated anymore (see #461)
- Correcting database type from MULTIPOINT to POINT (MySQL bug)

### Removed

- Merge the "components" window into the "about" window (see #488)



