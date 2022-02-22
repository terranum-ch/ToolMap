from conans import ConanFile, CMake
import os


class Toolmap(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = ["wxwidgets/3.1.4@terranum-conan+wxwidgets/stable",
                "wxpdfdocument/1.0.5@terranum-conan+wxpdfdocument/stable",
                "mysql/5.6.56@terranum-conan+mysql/stable",
                "gdal/3.3.3",
                "geos/3.10.2",
                "proj/8.2.1",
                "libtiff/4.0.9",
                ]

    generators = "cmake", "gcc", "txt"

    def configure(self):
        if self.settings.os == "Linux":
            self.options["wxwidgets"].webview = False # webview control isn't available on linux.
        self.options["gdal"].shared = True

    def imports(self):
        # copy libraries
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        # Don't copy dylib on OSX, bundle is created on cmake install . step

        # copy errmsg.sys on different places
        if self.settings.os == "Windows":
            self.copy("errmsg.sys", dst="bin/mysql", src="share/english")
        if self.settings.os == "Macos":
            self.copy("errmsg.sys", dst="bin/ToolMap.app/Contents/mysql", src="share/english")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install()

