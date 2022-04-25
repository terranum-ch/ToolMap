# LATEST BUILD INSTRUCTIONS #

## Conan

Install Conan, see https://docs.conan.io/en/latest/installation.html

Another option is to install pipx in order to easily deploy conan in a clean virtualenv. See https://pypa.github.io/pipx/
for more information

        python3 -m pip install --user pipx
        python3 -m pipx ensurepath
        pipx install conan

## Toolmap

From the Toolmap directory create a build folder :

        mkdir cmake-build-release
        cd cmake-build-release

Install the required libraries with Conan

        conan install .. 

or 

        conan install .. -o build_type=Debug

if a library is missing, add the `--build=missing` option.

Build using the following command

        conan build ..

If using an IDE, use the cmake and build from the IDE instead of the `conan build ..` command. In this case, run the following
command on OSX to fix the bundle with the correct dynamic library path

        cmake --install .

This step isn't needed when building using `conan build ..`.




