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

Build using the following command

        conan build ..

## Build options

The following options are supported by `conan install ..` :

| Option              | CMAKE equivalent  | Description                                                     |
|---------------------|-------------------|-----------------------------------------------------------------|
| -s build_type=Debug | ---               | Use the libraries and create the project in debug mode          |
| --build=missing     | ---               | Build the missing libraries                                     |
| -o unit_test=True   | -DUSE_UNITTEST=ON | Install the necessary libraries for the unit tests and run them |

## Working from an IDE

If using an IDE, use the cmake and build from the IDE instead of the `conan build ..` command.

On OSX, run the following command  to fix the bundle with the correct dynamic library path

        cmake --install .

This step isn't needed when building using `conan build ..`.





