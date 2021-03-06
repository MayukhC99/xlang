# Tutorial: Generate a native Python extension module with Python/WinRT

This tutorial is an adjunct to the [Create a WinML application with Python/WinRT tutorial](readme.md) that explains how to generate a native Python extension module using Python/WinRT. The [pywinrt_output](/pywinrt_output) folder conains the source code for an extension module generated by Python/WinRT that provides access to enough of the Windows Runtime API surface to build the [C++/WinRT WinML tutorial](https://docs.microsoft.com/en-us/windows/ai/get-started-desktop) in Python. This document explains how those files were generated. You can use these instructions to project other WinRT APIs for use from Python.

> Note, xlang in general and Python/WinRT in particular is very, very early in its development. It is not ready for general usage building Python applications against Windows Runtime APIs. Many features are missing or will be changed while development continues. Only the core scenario described below has been tested end-to-end. You are encouraged to experiment outside the bounds of the core scenario described in the [WinML tutorial](readme.md). Just remember that Python/WinRT is in active development as you experiment and please [open issues](https://github.com/Microsoft/xlang/issues) when (not if!) you find bugs or missing functionality.

## Prerequisites

* [Visual Studio 2017](https://developer.microsoft.com/windows/downloads), version 15.8 or later.
  * This tutorial was tested against Visual Studio 15.8.7, the latest version as of this writing.
  * Visual Studio's Desktop Development with C++ workload installation is required.
* [Windows 10](https://developer.microsoft.com/windows/downloads), version 1809 or later.
* [Windows SDK](https://www.microsoft.com/software-download/windowsinsiderpreviewSDK), build 17763 or later
  * Windows SDK, build 17763 can be installed as part of Visual Studio 15.8.7.
* [Python for Windows](https://www.python.org), version 3.6 or later
  * Visual Studio's Python Development workload is _not_ required, but does include Python 3.6.
* [Git for Windows](https://gitforwindows.org/)
* [CMake](https://cmake.org/), version 3.11 or later
  * Visual Studio's Desktop Development with C++ workload includes CMake 3.11
* [Ninja](https://ninja-build.org/), version 1.8 or later
  * Visual Studio's Desktop Development with C++ workload includes Ninja 1.8.2

## Clone the xlang repo

> Note, xlang is very, very early in its development. Eventually, you will not need to clone the repo and build the Python/WinRT tooling from source.

Currently, Python/WinRT is only distributed as source code. To use it, you'll need to build it. To build it, you'll need to clone the [xlang repo](https://github.com/Microsoft/xlang).

``` shell
git clone https://github.com/Microsoft/xlang.git
```

## Build Python/WinRT

> Note, Python/WinRT currently depends on the C++/WinRT language projection included in the Windows SDK, build 17763. When a cross-platform C++ language projection becomes available,  the Python language projection will be updated to use it.

You can build Python/WinRT from Visual Studio or from the command line.

In Visual Studio, select Open -> Folder... (Ctrl-Shift-Alt-O) and then select the root folder of the cloned xlang repository. Visual Studio will take a few moments to process the CMake files in the repo. When Visual Studio is ready, you can build the Python/WinRT tool by selecting CMake -> Build Only -> (Project) pywinrt.exe (tool\python\pywinrt.exe).

From the command line, you can build Python/WinRT from a developer command prompt. From the start menu, select "Developer Command Prompt for VS 2017". Change directory to the root of your cloned repo, and execute the following command.

``` shell
.\src\scripts\windows\build.cmd pywinrt
```

After building, the Python/WinRT tool will be available at  _build\Windows\x86\Debug\tool\python\pywinrt.exe (relative to the root of your xlang repo). Both Visual Studio and the windowsBuild.ps1 script are configured to build into the same location.

> Note, the xlang repo includes [Visual Studio Natvis visualization](https://docs.microsoft.com/en-us/visualstudio/debugger/create-custom-views-of-native-objects) for C++/WinRT types. Building this project requires the Visual Studio SDK to be installed. If you attempt to build the entire xlang project (from either Visual Studio or the command line), you will get an error stating that vsdebugeng.lib is missing if the VS SDK is not installed.

## Generate Python Projection required WinRT types

Now that you have a binary copy of pywinrt.exe, you will use it to generate a [Python native extension module](https://docs.python.org/3/extending/extending.html) that enables Python to access WinRT types. pywinrt.exe is a command line utility so you'll need to launch a command prompt. This tutorial will assume you've launched a VS 2017 Developer Command Prompt window.

pywinrt.exe has a set of command line parameters which are not yet documented. Here's a quick primer on pywinrt's command line parameters:

* -input - specifies the full path to a WinMD file to project in Python. At least one -input must be specified, but there is no limit to the number of input metadata files.
  * Example: `-input "%WindowsSdkDir%\References\10.0.17763.0\Windows.Foundation.FoundationContract\3.0.0.0\Windows.Foundation.FoundationContract.winmd"`
* -output - specifies the path to where the Python/WinRT extension module will be generated. Defaults to ".\output".
* -include - specifies a string for fully namespace qualified types to include in the extension module. If no -include arguments are specified, defaults to all the types in all the input metadata files. Zero or more include arguments are allowed.
  * Example: `-include Windows.Foundation`
* -exclude - specifies a string of fully namespace qualified types to exclude in the extension module. If no -exclude arguments are specified, pywinrt defaults to all the types in all the input metadata files. Zero or more exclude arguments are allowed.
  * Example: `-exclude Windows.Foundation.Metadata`
* -module - specifies the name of the Python extension module. Defaults to "pyrt". At most one module argument is allowed
  * Note, the native extension module uses an underscore prefix to the extension module name (i.e. _pyrt). Eventually, Python/WinRT will also produce a non-native Python module to provide a pythonic developer experience, but that is not implemented yet.
* -verbose - specifies to produce more command line information during projection generation. At most one module argument is allowed

From the root of your xlang repo, you can invoke pywinrt with the following command line to generate a Python native extension module that projects enough of the Windows Runtime API to build the [WinML tutorial](readme.md).

``` shell
_build\Windows\x86\Debug\tool\python\pywinrt.exe -input "%WindowsSdkDir%\References\10.0.17763.0\Windows.Foundation.FoundationContract/3.0.0.0/Windows.Foundation.FoundationContract.winmd" -input "%WindowsSdkDir%\References\10.0.17763.0\Windows.Foundation.UniversalApiContract/7.0.0.0/Windows.Foundation.UniversalApiContract.winmd" -input "%WindowsSdkDir%\References\10.0.17763.0\Windows.AI.MachineLearning.MachineLearningContract/1.0.0.0/Windows.AI.MachineLearning.MachineLearningContract.winmd" -include Windows.Foundation -include Windows.AI.MachineLearning -include Windows.Graphics.Imaging -include Windows.Media -include Windows.Storage -exclude Windows.Media.PlayTo
```

This command will generate the Python projection for the Windows.Foundation, Windows.AI.MachineLearning, Windows.Graphics.Imaging, Windows.Media (except for Windows.Media.PlayTo) and Windows.Storage namespaces into the /output subdirectory.

> Note, Python/WinRT currently flattens WinRT namespaces. Both Windows.Media and Windows.Media.PlayTo namespaces have a type named PlaybackRateChangeRequestedEventArgs. If the Windows.Media.PlayTo namespace is not excluded, the generated file will not compile since PlaybackRateChangeRequestedEventArgs will be specified twice. Hence, we need to exclude Windows.Media.Playto for now. Namespace support is coming in a future update to Python/WinRT.

From here, you can follow the directions in the original tutorial, only substituting the "output" folder wherever that document references "pywinrt_output".

> Note, you might notice that the instructions above generates significantly more files (93) that are included in the main tutorial's pywinrt_output folder (21). This is becuase the all sub-namespaces that were not needed for the [WinML tutorial](https://docs.microsoft.com/en-us/windows/ai/get-started-desktop) were explicitly excluded. This was done to minimize build time for anyone building the sample themselves. Excluding more namespaces means the generated extension module supports less Windows Runtime functionality, but builds quicker.