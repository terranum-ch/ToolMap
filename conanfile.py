from conans import ConanFile, CMake
import os


class Toolmap(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "wxwidgets/3.1.4@terranum-conan+wxwidgets/stable",
        "wxpdfdocument/1.0.5@terranum-conan+wxpdfdocument/stable",
        "mysql/5.6.51@terranum-conan+mysql/stable",
        "gdal/3.3.3",
        "geos/3.9.1",
        "proj/8.1.1",
        "libcurl/7.80.0",
    ]

    generators = "cmake", "gcc", "txt"

    def configure(self):
        self.options["gdal"].with_curl = True # for xml support
        if self.settings.os == "Linux":
            self.options["wxwidgets"].webview = False  # webview control isn't available on linux.
            self.options["gdal"].shared = True
        if self.settings.os == "Macos":
            self.options["gdal"].shared = True

    def imports(self):
        # copy libraries
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib", dst="bin", src="lib")
        # Don't copy dylib on OSX, bundle is created on cmake install . step

        # copy errmsg.sys on different places
        if self.settings.os == "Windows" or self.settings.os == "Linux":
            self.copy("errmsg.sys", dst="bin/mysql", src="share/english")
        if self.settings.os == "Macos":
            self.copy("errmsg.sys", dst="bin/ToolMap.app/Contents/mysql", src="share/english")

        # copy proj library datum
        if self.settings.os == "Windows" or self.settings.os == "Linux":
            self.copy("*", dst="bin", src="res", root_package="proj")
        if self.settings.os == "Macos":
            self.copy("*", dst="bin/ToolMap.app/Contents/share/proj", src="res", root_package="proj")

        # copy webfiles on different places
        _source_folder = os.path.join(os.getcwd(), "..")
        if self.settings.os == "Macos":
            self.copy("*.xml", dst="bin/ToolMap.app/Contents/share/toolmap",
                      src=os.path.join(_source_folder, "resource", "web"))

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install()
