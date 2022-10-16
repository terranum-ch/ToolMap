from conans import ConanFile, CMake


class FeatureTOC(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    # requires = ["wxwidgets/3.1.7@terranum-conan+wxwidgets/stable"]

    generators = "cmake", "gcc", "txt"

    def configure(self):
        if self.settings.os == "Linux":
            self.requires('wxwidgets/3.1.7@terranum-conan+wxwidgets/stable')
            self.options["wxwidgets"].webview = False  # webview control isn't available on linux.
        else:
            self.requires('wxwidgets/3.2.1@terranum-conan+wxwidgets/stable')

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib")  # From lib to bin

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
