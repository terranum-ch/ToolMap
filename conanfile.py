from conans import ConanFile, CMake
import os


class Toolmap(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "wxwidgets/3.2.5@terranum-conan+wxwidgets/stable",
        "mariadb/10.6.10@terranum-conan+mariadb/stable",
        "gdal/3.8.3@terranum-conan+gdal/stable",
        "wxpdfdocument/1.0.3-beta@terranum-conan+wxpdfdocument/stable",
        "libtiff/4.6.0",
        "libdeflate/1.19",
        "proj/9.3.1",
        "libjpeg/9e",
        "zlib/1.3",
    ]

    options = {
        "unit_test": [True, False],
        "code_coverage": [True, False]}

    default_options = {
        "unit_test": False,
        "code_coverage": False}

    generators = "cmake", "gcc", "txt"

    def requirements(self):
        if self.options.unit_test or self.options.code_coverage:
            self.requires("gtest/1.14.0")
        if self.settings.os == "Macos":
            self.requires("json-c/0.17")    # building json-c/0.16 isn't working on latest OSX.
        if self.settings.os == "Windows" or self.settings.os == "Macos":
            self.requires("libpng/1.6.43")

    def configure(self):
        if self.options.code_coverage:
            self.options.unit_test = True

        if self.settings.os == "Windows": # avoid linking issues
            self.options["zlib"].shared = True
            self.options["libjpeg"].shared = True
        
        self.options["gdal"].with_curl = True # for xml support
        self.options["gdal"].shared = True

        if self.settings.os == "Linux":
            self.options["wxwidgets"].png = "system"
            self.options['gdal'].with_png = False # to avoid static linking of libpng

    def imports(self):
        # copy libraries
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib", dst="bin", src="lib")
        if self.settings.os == "Linux":
            self.copy("*.so*", dst="bin", src="lib")

        # copy errmsg.sys on different places
        if self.settings.os == "Windows" or self.settings.os == "Linux":
            self.copy("errmsg.sys", dst="share/mysql", src="share/english")
        if self.settings.os == "Macos":
            self.copy("errmsg.sys", dst="bin/ToolMap.app/Contents/mysql", src="share/english")
            if self.options.unit_test:
                self.copy("errmsg.sys", dst="mysql", src="share/english")
        if self.options.code_coverage:
            self.copy("errmsg.sys", dst="share/mysql", src="share/english")

        # copy proj library datum
        if self.settings.os == "Windows" or self.settings.os == "Linux":
            self.copy("*", dst="share/proj", src="res", root_package="proj")
        # copy proj.db into the binary directory on Linux
        if self.settings.os == "Linux":
            self.copy("proj.db", dst="bin", src="res", root_package="proj")
        if self.settings.os == "Macos":
            self.copy("*", dst="bin/ToolMap.app/Contents/share/proj", src="res", root_package="proj")
            if self.options.unit_test:
                self.copy("*", dst="bin", src="res", root_package="proj")

        # copy xml webfiles
        _source_folder = os.path.join(os.getcwd(), "..")
        if self.settings.os == "Macos":
            self.copy("*.xml", dst="bin/ToolMap.app/Contents/share/toolmap",
                      src=os.path.join(_source_folder, "resources", "web"))
        else:
            self.copy("*.xml", dst="share/toolmap",
                      src=os.path.join(_source_folder, "resources", "web"))

    def build(self):
        cmake = CMake(self)
        if self.options.unit_test:
            cmake.definitions["USE_UNITTEST"] = "ON"
        if self.options.code_coverage:
            cmake.definitions["USE_UNITTEST"] = "ON"
            cmake.definitions["USE_CODECOV"] = "ON"

        cmake.configure()
        cmake.build()

        if self.settings.os == "Macos":
            cmake.install()
