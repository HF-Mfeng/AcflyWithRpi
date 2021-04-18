# AcflyWithRpi

## 1、功能实现

> 与树莓派串口通信
>
> 树莓派指令控制速度、起飞、降落、离前方墙距离悬停
>
> 实现TOFSense激光驱动

## 2、文件介绍

> 从ACFLY官方资料20210224版本更改而来
>
> （1）更改文件
>
> - /Modes/M35_Auto1.cpp
>
>   > 以#ifdef Ourselves_M35_Auto1_cpp包围编译的属于自行修改文件，其他属于未改文件
>
> （2）新增文件
>
> - /User/raspberry.cpp 和 raspberry.h
>
>   > 使用串口8与树莓派收发数据，包括设置波特率，发送数据，接收数据并去报头
>   >
>   > 报头：QQ（个数不定）
>   >
>   > 报尾：W  （个数不定）
>
> - /User/LMZLink.hpp
>
>   > 定义了通信协议，由一个mode（int）控制，和两个容器(vector<string>, vector<double>) 组成，mode控制模式，容器存储数据标签和数据值
>
> - /User/LMZPID.hpp
>
>   > 定义了一个c++的PID类，分为位置式与增量式控制，由枚举ways进行选择。
>   >
>   > 还分为差距控制式和量值控制式PID调节
>
> - /User/TOF.hpp
>
>   > 实现了TOFSense的串口驱动，可以获取测距信息，>0有效
>
> - /User/ACFLY_cmd.hpp
>
>   > 最主要的文件
>   >
>   > 功能并未完全实现，目前实现了（三轴速度控制，前方定距，起飞降落）
>   >
>   > 需要完善