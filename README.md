# APKmenuTOOL - 安卓右键工具

![](./screenshot.png)


## **安卓右键工具v1.2版本使用说明**
安卓右键工具是一款集合复制路径，Dex转Jar，Manifest和AXML查看，APK相关信息提取、优化、签名、反编译、回编译，手机信息查看、屏幕截图等功能的综合型工具，同时本工具拥有自定义插件功能，方便使用者对本工具的功能进行扩充。



### 一、运行环境
1. Windows操作系统64位
2. 需安装Java开发环境并配置环境变量
3. 需安装Python并配置环境变量，支持Python3和Python2
4. 需安装ADB并配置环境变量

### 二、安装和卸载说明
#### 1、安装
运行APKmenuTOOL.exe，点击“添加到系统右键菜单”，安装完毕。默认安装路径为D:\APKmenuTOOL。

#### 2、卸载
运行APKmenuTOOL.exe，点击“从系统右键菜单移除”，卸载完毕。文件目录请手动删除。

### 三、使用方法
使用该工具时需选中文件，右击，在弹出的菜单项中选择安卓右键工具，然后选择需要使用的功能即可。


### 四、功能说明

#### 1、复制路径
复制被选中文件的路径（全文件路径，支持多个文件）到剪贴板，执行结果将在右下角冒泡提示。

#### 2、Dex->Jar
将dex(或APK中的dex）文件转为jar文件。任务完成时会在当前目录下生成文件名为源文件名的jar文件，并自动使用jd-gui打开所生成的jar文件。执行结果将在右下角冒泡提示。

#### 3、Manifest->TXT|AXML->XML
自动提取APK文件中的AndroidManifest.xml文件内容（若为AXML文件则直接提取文件内容），以TXT文件格式保存到当前目录，并自动打开生成的TXT文件。执行结果将在右下角冒泡提示。

#### 4、查看APK信息
自动提取APK文件中软件名称、包名、版本、启动Activity、权限列表等信息。任务完成时自动显示获取的信息，执行结果将在右下角冒泡提示。
  
#### 5、查看签名信息
自动提取APK文件的签名信息，任务完成时在当前目录下生成文件名为”源文件名_signinfo“的文本文件，，并自动打开生成的TXT文件。执行结果将在右下角冒泡提示。
  
#### 6、签名
对选中的APK文件进行签名。默认使用debug签名，签名文件可以自由配置，路径为D:\APKmenuTOOL\tools\sign。任务完成时将在当前目录下生成文件名为“源文件名”的APK文件和文件名为“源文件名_sign”的log文件。执行结果将在右下角冒泡提示。

#### 7、安装（卸载安装）
自动卸载被选中APK，然后将该APK安装到手机。使用该功能时，请先连接手机设备。执行结果将在右下角冒泡提示。

#### 8、安装（替换安装）
将被选中APK覆盖安装到手机。使用该功能时，请先连接手机设备。执行结果将在右下角冒泡提示。

#### 9、卸载
从手机中卸载被选中APK文件。使用该功能时，请先连接手机设备。执行结果将在右下角冒泡提示。

#### 10、查看加壳信息
检测APK文件是否被加壳，若被加壳则检测加壳厂家。任务完成时自动显示获取的信息，执行结果将在右下角冒泡提示。

#### 11、 APK深度检测
自动检测APK文件是否是游戏APP和是否为XPOSED插件。如果为游戏APP则显示游戏引擎，如果为XPOSED插件APP则显示入口类。

#### 12、查看手机信息
自动获取手机类型、系统版本、系统API版本、系统CPU类型、序列号、IMEI、分辨率等信息，任务完成后将自动显示获取到的手机信息，使用该功能时请先连接手机设备。执行结果将在右下角冒泡提示。

#### 13、手机截图
对手机当前画面进行截图，任务完成时会在当前目录下生成screenshot.png，同时复制截图至剪贴板。执行结果将在右下角冒泡提示。
  
#### 14、提取图标
提取源文件APK中的图标，任务完成时会在当前目录下生成APK中所有分辨率的目标文件。执行结果将在右下角冒泡提示。
  
#### 15、zipalign优化
对源文件进行优化，使程序运行更快。任务完成时会在当前目录下生成文件名为“源文件名_align”的APK文件。执行结果将在右下角冒泡提示。

#### 16、反编译
对被选中APK进行反编译。任务完成时会在当前目录下生成和被选中APK名字一样的文件夹。执行结果将在右下角冒泡提示。

#### 17、回编译
对反编译后的APK文件夹进行回编译得到APK文件。任务完成时会在当前目录下生成编译后的APK文件。执行结果将在右下角冒泡提示。

#### 18、提取so
提取被选中APK中lib目录下的所有后缀为so的文件。任务完成时会在当前目录下生成名为“源文件名_lib”的文件夹。执行结果将在右下角冒泡提示。

#### 19、lib规范检查
显示被选中APK中lib目录下so文件的组织结构，并判断lib目录下的SO文件配置是否规范。

#### 20、自定义插件
预留的可安装插件的接口。用户可以通过以下步骤安装自定义插件:
  
1. 修改D:\APKmenuTOOL\tools\config\config.ini 文件中的CustomMenu下的name属性，多个插件以“-”分隔。
2. 修改D:\APKmenuTOOL\tools\python\CustomPlug.py（安装了Python3的用户请修改CustomPlug3.py）文件。
  
**请注意：**
 - filepath为选中的文件或者文件夹路径。 
 - toolspath为右键工具安装目录下的tools目录，里面为常用工具
 - 函数内部不能使用print等函数向控制台打印数据
 - 返回必须是字典，且要求键为result，如 result = {'result':u'我是返回的结果'}

#### 21、关于v1.2
关于网易易盾加固的介绍说明。
  
### 四、联系我们
如有问题，请从官网联系我们：[网易易盾加固](http://dun.163.com/product/app-protect)

网易易盾加固提供Android应用加固、iOS应用加固、手游智能反外挂、SDK加固、安全组件等多种服务产品。高安全性、高兼容性、高稳定性、操作便捷、灵活定制、国际认证是我们产品的核心优势。通过防逆向、防篡改、防调试、混淆等手段持续不断的为用户产品安全保驾护航。
