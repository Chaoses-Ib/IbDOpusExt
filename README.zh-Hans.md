# IbDOpusExt
语言: [English](README.md), [简体中文](README.zh-Hans.md)  
一个 [Directory Opus](https://www.gpsoft.com.au/) 的扩展。

## 功能
### 命令
<table>
<thead><tr>
    <th>命令</th>
    <th>说明</th>
    <th>例子</th>
</tr></thead>
<tbody>
    <tr>
        <td>#Set MaxThumbSize</td>
        <td>修改缩略图的最大尺寸</td>
        <td><pre lang="Batchfile">#Set MaxThumbSize = 512</pre></td>
    </tr>
</tbody>
</table>

### 配置
创建 /dopusdata\ConfigFiles\Plugins\IbDOpusExt\config.yaml ：
```yaml
# YAML
FileOperations:
  Logging:
    UndoLog:
      # 最大 255
      MaxItems: 10
Folders:
  FolderBehaviour:
    # 要求 Everything 开启“索引文件夹大小”选项（位于 选项\索引）
    GetSizesByEverything: false
```
（UTF-8 编码）

## 支持版本
* Directory Opus 12.23 64位

## 安装
1. 从 [Releases](../../releases) 下载发行文件。
1. 把 IbDOpusExt.dll 放进 C:\Program Files\GPSoftware\Directory Opus\Viewers 。
1. 打开 DOpus，进入 设置→配置→查看器→插件，勾选 IbDOpusExt，点击“确认”按钮。

## 鸣谢
本项目使用了以下库：

* [Detours](https://github.com/microsoft/detours)
* [[Boost::ext].DI](https://github.com/boost-ext/di)
* [eventpp](https://github.com/wqking/eventpp)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)