# IbDOpusExt
Languages: [English](README.md), [简体中文](README.zh-Hans.md)  
An extension for [Directory Opus](https://www.gpsoft.com.au/).

## Features
Add commands:
Command | Description | Example
------- | ----------- | -------
@ibext | Turn on command extension | <pre lang="Batchfile">@ibext</pre>
Set MaxThumbSize | Modify the max size of thumbnails | <pre lang="Batchfile">@ibext<br />Set MaxThumbSize = 512</pre>

## Supported Version
* Directory Opus 12.23 64-bit

## Installation
1. Put IbDOpusExt.dll into C:\Program Files\GPSoftware\Directory Opus\Viewers .
1. Open DOpus, go Settings→Preferences→Viewer→Plugins, check IbDOpusExt, click the OK button.

## Credits
This project uses the following libraries:

* [Detours](https://github.com/microsoft/detours)
* [[Boost::ext].DI](https://github.com/boost-ext/di)
* [eventpp](https://github.com/wqking/eventpp)
