from conans import ConanFile, CMake
import os


class Toolmap(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "wxwidgets/3.2.1@terranum-conan+wxwidgets/stable",
        "mariadb/10.6.10@terranum-conan+mariadb/stable",
        "gdal/3.5.1@terranum-conan+gdal/stable",
        "wxpdfdocument/1.0.3-beta@terranum-conan+wxpdfdocument/stable",
        "libtiff/4.4.0",
        "libdeflate/1.12",
        "proj/9.0.0",
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
            self.requires("gtest/cci.20210126")

    def configure(self):
        if self.options.code_coverage:
            self.options.unit_test = True
        
        self.options["gdal"].with_curl = True # for xml support
        self.options["gdal"].shared = True


        # this isn't needed anymore with wxWidgets 3.2.1
        # if self.settings.os == "Linux":
        #    self.options["wxwidgets"].webview = False  # webview control isn't available on linux.

    def imports(self):
        # copy libraries
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib", dst="bin", src="lib")
        if self.settings.os == "Linux":
            self.copy("*.so*", dst="bin", src="lib")

        # copy errmsg.sys on different places
        if self.settings.os == "Windows" or self.settings.os == "Linux":
            self.copy("errmsg.sys", dst="bin/mysql", src="share/english")
        if self.settings.os == "Macos":
            self.copy("errmsg.sys", dst="bin/ToolMap.app/Contents/mysql", src="share/english")
            if self.options.unit_test:
                self.copy("errmsg.sys", dst="mysql", src="share/english")

        # copy proj library datum
        if self.settings.os == "Windows" or self.settings.os == "Linux":
            self.copy("*", dst="bin", src="res", root_package="proj")
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
