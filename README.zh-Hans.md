# IbDOpusExt
语言: [English](README.md), [简体中文](README.zh-Hans.md)  
一个 [Directory Opus](https://www.gpsoft.com.au/) 的扩展。

## 功能
增加命令：
命令    |  说明 |  例子
------- | ---- | ------
@ibext | 开启命令扩展 | <pre lang="Batchfile">@ibext</pre>
Set MaxThumbSize | 修改缩略图的最大尺寸 | <pre lang="Batchfile">@ibext<br />Set MaxThumbSize = 512</pre>

## 支持版本
* Directory Opus 12.23 64位

## 安装
1. 把 IbDOpusExt.dll 放进 C:\Program Files\GPSoftware\Directory Opus\Viewers 。
1. 打开 DOpus，进入 设置→配置→查看器→插件，勾选 IbDOpusExt，点击“确认”按钮。

## 鸣谢
本项目使用了以下库：

* [Detours](https://github.com/microsoft/detours)
* [[Boost::ext].DI](https://github.com/boost-ext/di)
* [eventpp](https://github.com/wqking/eventpp)
