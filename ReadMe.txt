﻿0.1
C#语言开发，适用于windows .net4.0+
使用网页加载登录界面，手动输入账号密码登陆，关闭时进入后台，最小化到托盘区
0.2
网页内核改为WebKit
1.0
python3开发，适用于windows、linux、mac
使用表单自动登录BJUT网关，并通过定时的检查登录状态。

1.1
将账号信息放置到配置文件中，配置文件可以在etc目录或用户目录或程序目录中。

1.2
修复无法读取用户主目录的问题。可以安装服务到系统，开机启动。

1.3
修复登录ipv6时查不到状态的问题。
若登录返回跳转网页，则继续解析跳转网页。
登录时使用异常处理。

1.4
BjutNet构造函数，失败时抛出异常
账号信息每隔半小时输出一次
2.0.0
使用QT开发，目前只使用于windows，后续会开发linux，mac版本。
设计窗口界面，在窗口显示流量信息，软件日志，账号信息，以及保存、下线按钮。
软件启动后在系统托盘区显示图标，关闭窗口后，依然可以对托盘操作：点击显示流量信息，双击打开窗口，右击打开菜单。
账号登陆类型为ipv4+ipv6时，依次检测lgn6.bjut.edu.cn和lgn.bjut.edu.cn，
账号登陆失败后，重试间隔以2的n次幂增长，最长1024s。
使用install.bat写脚本，实现安装功能（桌面快捷方式、开机启动、注册表）

2.1.0
开发Ubuntu x64版本
install可以在系统中创建启动的快捷方式，由于带有图形界面，无法安装到服务启动
软件只允许启动一个实例，多次启动则唤醒原来的窗口
接受参数-tray，只启动托盘，而不显示主窗口
主窗口隐藏时，不更新状态信息，减少计算量，显示时，立即获取保存的流量状态，更新页面，若立即从网络获取会造成界面卡顿
消除代码unused parameter警告
修复部分windows下，程序依赖组件不全的问题

2.2.0
增加bjut_wifi登录功能
统一换行符为\n
添加网络类型的判断，有线还是无线？
登录函数区分有线和无线，且可以指定登录类型
检测状态时判断网络类型
登录时再次验证状态，根据状态登录网关

2.2.1
QT更新为5.9.6
添加上线按钮
网络状态切换时，自动检测并登录账号
修复登录ipv4或6时，消息文字不正确的问题
修复点击应用后无法立即上线的问题
修复点击应用后，账号信息未重新加载的问题

2.3.0
修复已经上线但登录失败后，点击上线不能立刻反应的问题
托盘右键菜单添加上线和下线按钮。

//网络切换，检测是否可上网。
//上线时，检测是否可上网

//安装包程序
//添加业务办理功能

