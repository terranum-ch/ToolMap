from conans import ConanFile, CMake


class FeatureTOC(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "gcc", "txt"

    options = {"old_linux_wxwidgets": [True, False]}
    default_options = {"old_linux_wxwidgets": False}

    def configure(self):
        if self.settings.os == "Linux":
            if self.options.old_linux_wxwidgets:
                self.requires('wxwidgets/3.1.7@terranum-conan+wxwidgets/stable')
                self.options["wxwidgets"].webview = False  # webview control isn't available on linux.
            else:
                self.requires('wxwidgets/3.2.1@terranum-conan+wxwidgets/stable')
        else:
            self.requires('wxwidgets/3.2.1@terranum-conan+wxwidgets/stable')

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")  # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib")  # From lib to bin

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
