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
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst=os.path.join("bin", "ToolMap.app", "Contents", "lib"), src="lib")  # From lib to bin

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        print (self.build_folder)
