# IbDOpusExt
Languages: [English](README.md), [简体中文](README.zh-Hans.md)  
An extension for [Directory Opus](https://www.gpsoft.com.au/).

## Features
* File Display Modes
  * Thumbnails
    * `#Set MaxThumbSize` command  
      Dynamically modify the max size of thumbnails.  
      e.g. `#Set MaxThumbSize = 512`
* File Operations
  * Logging
    * Configure maximum item number of undo log
* Folders
  * Folder Behaviour
    * Size column via Everything  
      Display folder sizes instantly with voidtool's [Everything](https://www.voidtools.com/).
* Viewer
  * Plugins
    * Fix dependency loading bug for plugins
* Zip & Other Archives
  * Archive and VFS Plugins
    * Fix dependency loading bug for plugins

## Requirements
* Windows 10 or later
* Directory Opus v12.23 x64
* [.NET 5 Runtime](https://dotnet.microsoft.com/download/dotnet/thank-you/runtime-desktop-5.0.8-windows-x64-installer)
* [VC++ 2019 Runtime x64](https://support.microsoft.com/topic/the-latest-supported-visual-c-downloads-2647da03-1eea-4433-9aff-95f26a218cc0)

## Installation
1. Download release package from [Releases](../../releases).
1. Extract the package and put the files into `C:\Program Files\GPSoftware\Directory Opus` .
1. Open DOpus, go Settings→Preferences→Viewer→Plugins, check IbDOpusExt, click the Apply button.

## Credits
This project uses the following libraries:

* [[Boost::ext].DI](https://github.com/boost-ext/di)
* [Detours](https://github.com/microsoft/detours)
* [eventpp](https://github.com/wqking/eventpp)
* [IbEverythingLib](https://github.com/Chaoses-Ib/IbEverythingLib)
* [IbWinCppLib](https://github.com/Chaoses-Ib/IbWinCppLib)