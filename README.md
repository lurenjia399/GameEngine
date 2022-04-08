# GameEngine
 directx12和游戏引擎学习

# DX12的一些小概念

## CommandAllocator  GraphicsCommandList  CommandQueue

1 我们可以向命令列表中添加命令，由cpu将命令列表中的命令提交到命令队列中，gpu就会按照命令队列的顺序依次执行命令，**需要注意的是当命令队列满的时候，cpu就无法在继续提交处于空闲状态，而当命令队列空的时候，gpu就会处于空闲状态**，，

2 下一个概念就是命令分配器，，，他是与命令列表相关联，存储在命令列表中的命令实际上是存储到了命令分配器中。**我们可以创建出多个命令列表关联一个命令分配器，但命令列表不能同时存储命令。**原因是：需要保证命令列表中的数据按照顺序添加到命令分配器中，所以与关联相同命令分配器的命令列表，其中只能有一个命令列表有命令。

3 我们在将命令列表中的命令添加到命令队列中后，就可以执行命令列表.Reset()方法，清空命令列表，就可以复用这个命令列表来存储其他命令。

但是不能将命令分配器清空，因为在将命令提交给命令队列后，命令队列有可能会引用命令分配器中的数据，再加上gpu并不一定会立即执行，所以必须保证gpu在执行完名列队列中的命令后，再将命令分配器清空。

## D3dDevice

驱动可做的事情很多：

可以帮助我们来创建一些东西：CreateCommandList，CreateCommandQueue，CreateCommandAllocator，CreateFence，CreateDepthStencilView

## Fence

GPU和CPU的同步处理

## 超级采样SSAA 多重采样MSAA

超级采样：我们在确定屏幕上每一个像素的颜色的时候，将像素等分成4份子像素，然后分别确定每个子像素的颜色，然后在取4个子像素颜色的平均值当作像素颜色，这样做的化就会计算4次颜色，并且后台缓冲区以及深度缓冲区就会变成原来的4倍，加大了计算力度。

多重采样：我们在确定屏幕上每一个像素的颜色的时候，同样的也将像素分成4份子像素，与SSAA不同的是，我们首先获取像素中心位置的颜色ColorCenter，然后将ColorCenter赋值给4个子像素，接下来判断像素的可见性（通过深度模板测试），然后取4个子像素颜色的平均值，这样做的化就会计算1次颜色，大大减少了计算力度。

## 交换链

一种数据结构，它模拟了双缓冲的设计模式，，，在渲染过程中有两个缓冲区（前台缓冲区和后台缓冲区），我们在显示前台缓冲区所展示的内容的时候，会同时往后台缓冲区写入下一帧的数据，在后台缓冲区完成后会将前台后台交换，这样无缝切换减少画面闪烁的情况。

## 资源描述符

resource：在渲染出一幅画面的过程中所需要的各种数据（前台缓冲区、后台缓冲区、各种贴图、深度模板缓冲区等）

resource descriptor：这些资源对gpu来说只是一堆数据，gpu无法分辨出这些资源的类型、用途、大小、格式等信息，所以就有了资源描述符，用来描述每个资源的信息，使得gpu知道这些这些管线中的资源怎么处理

descriptor heap：用来存储同一种资源描述符资源描述符，可以看成资源描述符数组
