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
* 文件操作
  * 日志
    * 撤销记录的数量上限
* 文件夹
  * 文件夹行为
    * 计算文件夹尺寸时直接从 Everything 获取

## 支持版本
* Directory Opus 12.23 64位

## 安装
1. 从 [Releases](../../releases) 下载压缩包。
1. 解压压缩包，把文件替换进 C:\Program Files\GPSoftware\Directory Opus 。
1. 打开 DOpus，进入 设置→配置→查看器→插件，勾选 IbDOpusExt，点击“确认”按钮。

## 鸣谢
本项目使用了以下库：

* [Detours](https://github.com/microsoft/detours)
* [[Boost::ext].DI](https://github.com/boost-ext/di)
* [eventpp](https://github.com/wqking/eventpp)