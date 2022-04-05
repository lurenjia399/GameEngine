# GameEngine
 dx和游戏引擎学习

# DX12初始化

## Debug

接受一些dx的日志信息

## DXGIFactory

和项目中的EngineFactory类似，，，应用**工厂模式设计模式**创建相应的东西

## CommandAllocator

将GraphicsCommandList中的命令列表存储到此分配器中，，，然后将命令分配至CommandQueue中。。。每帧都需要重新输入命令，重新分配，重新绘制，

## GraphicsCommandList

命令列表，程序传递给gpu的渲染命令，，，绘制一帧所需要的各种命令

## CommandQueue

执行队列中每项命令

## D3dDevice

驱动可做的事情很多：

可以帮助我们来创建一些东西：CreateCommandList，CreateCommandQueue，CreateCommandAllocator，CreateFence，CreateDepthStencilView

## Fence

GPU和CPU的同步处理

## 多重采样

自己设置采样方式

## 交换链

前后交换链，，**双缓冲的设计模式**，前用来展示，然后给后输入数据，等到后准备好后前后交换，，，交换相应的前后指针。

## 资源描述符

设置常量缓冲区，访问视图
