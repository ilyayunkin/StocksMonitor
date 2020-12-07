[![Build status](https://ci.appveyor.com/api/projects/status/92o26lwxgmypsd5e?svg=true)](https://ci.appveyor.com/project/ilyayunkin/stocksmonitor)
[![Build Status](https://travis-ci.com/ilyayunkin/StocksMonitor.svg?branch=master)](https://travis-ci.com/ilyayunkin/StocksMonitor)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/22afe098274a415794b80def1b151390)](https://www.codacy.com/gh/ilyayunkin/StocksMonitor/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ilyayunkin/StocksMonitor&amp;utm_campaign=Badge_Grade)
[![codecov](https://codecov.io/gh/ilyayunkin/StocksMonitor/branch/master/graph/badge.svg?token=0AZSC6W43C)](https://codecov.io/gh/ilyayunkin/StocksMonitor)

# StocksMonitor

## Structure
On the highest level, the project consists of several sub-projects that are built independently.
* Plugins,
* Application.
The plugins are dynamic libraries detected by the application in runtime. The purpose of the plugins is to provide URLs to web-pages and to parse the pages. 
One plugin has specific meaning because it provides the currency courses which can be used for dynamic currency conversion.

The application contains the rest of the logic and implements the functionality. It loads plugins after launch.

## Testing
Unit-tests and benchmarks are provided in the Test dirrectory. They can me invoked:
* From QtCretor.
* With "make check" command.

## Test coverage
Test coverage statistics is collected during Appveyor for Linux sessions. Several measures applied to get more real test coverage score:
1. The whole project is built with coverage compiler and linker flags to get coverage files for all sources.
1. Unit-test and benchmarks are separated in different projects because they have different purposes and require different approaches.
1. Benchmarks are built without the coverage flags because benchmarks are not supposed to check the application's functionality.
1. Coverage files related to unit-test classes are deleted to not count unit tests as software covered with tests.
1. Codecov bash uploader is called. It calls gcov, collects coverage files from folders, and uploads them to the server.

## Credits
- Qt framework
- SQLITE