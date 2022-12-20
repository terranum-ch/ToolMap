# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

- None

## [4.0.XXX - 2022-XX-XX] - NUGLAR

### Added

- Adding code coverage (see #478)
- Using conan for the build system (see #466)

### Changed

- New application logo (see #486)
- Using SVG for toolbar's icon instead of bitmaps (see #487)
- complete redesign of the table of contents to allow layers grouping (see #477, #178, #467)
- Improving the crash report email, adding the Toolmap's version (see #483)
- Moving from MySQL 5.6.51 to MariaDB 10.6.10 (see #471)
- bumping GDAL to 3.5.2
- bumping wxWidgets to 3.2.1
- Moving to gtest for unit testing (see #472)
- Moving ToolbasView to a separate project

### Fixed

- Symbology isn't duplicated anymore (see #461)
- Correcting database type from MULTIPOINT to POINT (MySQL bug)

### Removed

- Merge the "components" window into the "about" window (see #488)



