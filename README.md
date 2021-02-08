# IbDOpusExt
Languages: [English](README.md), [简体中文](README.zh-Hans.md)  
An extension for [Directory Opus](https://www.gpsoft.com.au/).

## Features
### Add commands
<table>
<thead><tr>
    <th>Command</th>
    <th>Description</th>
    <th>Example</th>
</tr></thead>
<tbody>
    <tr>
        <td>@ibext</td>
        <td>Turn on command extension</td>
        <td><pre lang="Batchfile">@ibext</pre></td>
    </tr>
    <tr>
        <td>Set MaxThumbSize</td>
        <td>Modify the max size of thumbnails</td>
        <td><pre lang="Batchfile">@ibext
Set MaxThumbSize = 512</pre></td>
    </tr>
</tbody>
</table>

### Extra configuration
Create /dopusdata\ConfigFiles\Plugins\IbDOpusExt\config.yaml :
```yaml
# YAML
FileOperations:
  Logging:
    UndoLog:
      # The max is 255
      MaxItems: 10
```
(UTF-8 encoding)

## Supported Version
* Directory Opus 12.23 64-bit

## Installation
1. Download release files from [Releases](../../releases).
1. Put IbDOpusExt.dll into C:\Program Files\GPSoftware\Directory Opus\Viewers .
1. Open DOpus, go Settings→Preferences→Viewer→Plugins, check IbDOpusExt, click the OK button.

## Credits
This project uses the following libraries:

* [Detours](https://github.com/microsoft/detours)
* [[Boost::ext].DI](https://github.com/boost-ext/di)
* [eventpp](https://github.com/wqking/eventpp)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)