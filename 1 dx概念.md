# 1 DX12的一些小概念

##  CommandAllocator  GraphicsCommandList  CommandQueue

1 我们可以向命令列表中添加命令，由cpu将命令列表中的命令提交到命令队列中，gpu就会按照命令队列的顺序依次执行命令，**需要注意的是当命令队列满的时候，cpu就无法在继续提交处于空闲状态，而当命令队列空的时候，gpu就会处于空闲状态**，，

2 下一个概念就是命令分配器，，，他是与命令列表相关联，存储在命令列表中的命令实际上是存储到了命令分配器中。**我们可以创建出多个命令列表关联一个命令分配器，但命令列表不能同时存储命令。**原因是：需要保证命令列表中的数据按照顺序添加到命令分配器中，所以与关联相同命令分配器的命令列表，其中只能有一个命令列表有命令。

3 我们在将命令列表中的命令添加到命令队列中后，就可以执行命令列表.Reset()方法，清空命令列表，就可以复用这个命令列表来存储其他命令。

但是不能将命令分配器清空，因为在将命令提交给命令队列后，命令队列有可能会引用命令分配器中的数据，再加上gpu并不一定会立即执行，所以必须保证gpu在执行完名列队列中的命令后，再将命令分配器清空。

### 为什么要有commandallocator呢？

gpu通过执行命令来实现渲染的操作，而这些命令是通过cpu传过来的。也就是说cpu需要将命令列表中的命令放到显存中 ，而gpu需要从显存中读取命令在执行，这样就对显存有了一读一取的操作，，，在多线程的情况下就会有同步的问题，所以就有了commandallocator来管理显存这块存放命令的区域。这就好比我在饭店点菜，服务员将我点的菜记录到纸上，点完之后，服务员将菜单递交给后厨，后厨根据每道菜依次做，这里的纸就好比是commandallocator。

## 围栏Fence（cpu线程和gpu线程间的同步）

为了充分利用多线程渲染的高性能，draw call 变成异步的，executeCommandlists也变成异步的了（cpu在将这些命令放到命令队列中后，就不管了，可以执行其他的操作了。gpu就可以根据自己的顺序来执行这些命令。），那么就会有一个问题：cpu如何知道我的这些命令（或者是哪写命令）被执行了? 这就引出了围栏的概念。Fence就是一个同步对象，用来同步gpu线程和cpu线程。

主要实现原理就是，为gpu线程设置一个初始的围栏值，接着为这个值在设置一个cpu事件句柄，然后gpu继续执行任务，而cpu就等待，等到gpu达到这个围栏值的时候，，就唤醒事件，cpu就知道gpu的工作已经做完了，改cpu执行操作了。

## 资源屏障 resource barrier（gpu线程间的同步）

在渲染流程的中，我们可能对某个资源先读后写，比如DepthStencilBuffer，但是当gpu还没有对资源写完，就读取的化，就会造成**资源冒险**，有可能引发问题。所以就有了ResourceBarrier，告诉gpu当前资源是正在处于资源转换状态，从状态1 transition to 状态2。

## 超级采样SSAA 多重采样MSAA

超级采样：我们在确定屏幕上每一个像素的颜色的时候，将像素等分成4份子像素，然后分别确定每个子像素的颜色，然后在取4个子像素颜色的平均值当作像素颜色，这样做的化就会计算4次颜色，并且后台缓冲区以及深度缓冲区就会变成原来的4倍，加大了计算力度。

多重采样：我们在确定屏幕上每一个像素的颜色的时候，同样的也将像素分成4份子像素，与SSAA不同的是，我们首先获取像素中心位置的颜色ColorCenter，然后将ColorCenter赋值给4个子像素，接下来判断像素的可见性（通过深度模板测试），然后取4个子像素颜色的平均值，这样做的化就会计算1次颜色，大大减少了计算力度。

## 交换链

一种数据结构，它模拟了双缓冲的设计模式，，，在渲染过程中有两个缓冲区（前台缓冲区和后台缓冲区），我们在显示前台缓冲区所展示的内容的时候，会同时往后台缓冲区写入下一帧的数据，在后台缓冲区完成后会将前台后台交换，这样无缝切换减少画面闪烁的情况。

## 资源描述符

resource：在渲染出一幅画面的过程中所需要的各种数据（前台缓冲区、后台缓冲区、各种贴图、深度模板缓冲区等）

resource descriptor：这些资源对gpu来说只是一堆数据，gpu无法分辨出这些资源的类型、用途、大小、格式等信息，所以就有了资源描述符，用来描述每个资源的信息，使得gpu知道这些这些管线中的资源怎么处理

descriptor heap：用来存储同一种资源描述符资源描述符，可以看成资源描述符数组

## 根签名

它分为：RootDescriptorTable(根描述符表)、RootConstant(根常量)、RootDescriptor(根描述符)。RootDescriptorTable最省资源，每个描述符表只占用1DWORD。而每个RootDescriptor占用2DWORD。每个根常量占用1DWORD。

描述符表需要创建CBV堆，并把CBV存入堆中，然后将CBV与资源绑定，在我们之前案例中就是这么做的。我们都会先创建一个CBV堆，然后根据CBV在堆中的地址而创建CBV。最终在绘制前设置下RootDescriptorTable即可。

```c++
{
    // 项目中使用了三个，都是贴图类的
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV = {};
    DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, TextureCount, 2);

    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV = {};
    DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeShadowMapSRV = {};
    DescriptorRangeShadowMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

    RootParam[5].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, 															      		D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);//texture
    RootParam[6].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV,    				    			                               		D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);//cubemap
    RootParam[7].InitAsDescriptorTable(1, &DescriptorRangeShadowMapSRV,                                                         	 	D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL);//shadowmap
}

{
    // 这部分就是创建SRV，需要有个描述符堆，堆里有描述符handle，指向SRV
    //构建Texture
    TextureShaderResourceView->BuildTextureShaderResource(DescriptorHeap.GetHeap(), 0);
    //构建cubeMap
    CubeMapResourceView->BuildTextureShaderResource(DescriptorHeap.GetHeap(), GetDrawTextureObjectCount());
}

{
    // 在渲染的时候，需要绑定下，第一个参数是第几个根参数，第二个参数是堆里面的handle，需要注意是否要偏移
    UINT HandleSize = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->				              			GetGPUDescriptorHandleForHeapStart());
        GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, Handle);
}
```

根描述符则不需要创建CBV堆，而是直接设置根描述符即可指示要绑定的资源。

```c++
{
    // 项目中是这么写的，规定了第几个参数在哪个寄存器中
    RootParam[0].InitAsConstantBufferView(0);//meshobject
    RootParam[1].InitAsConstantBufferView(1);//viewport
    RootParam[2].InitAsConstantBufferView(2);//light
    RootParam[3].InitAsConstantBufferView(3);//fog
    RootParam[4].InitAsShaderResourceView(0, 1);//material
}

{
    // 拿视口的举例了，创建常量缓冲区
    void FGeometryMap::BuildViewportConstantBufferView(UINT InViewportOffset)
    {
        ViewportConstantBufferView.CreateConstant(sizeof(FViewportTransformation), 
            1 + //主视口
            GetDynamicReflectionViewportNum() + //这个是动态反射的视口
            1 + // 阴影的视口
            InViewportOffset);
    }
}

{
 // 绘制的时候的绑定，第一个参数是哪个寄存器，第二个参数就是资源在显存中的地址
 GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportConstantBufferView.GetBuffer()->GetGPUVirtualAddress());
}
```

根常量也可以直接绑定资源，但是一个根常量只能绑定一个32位的常量值，空间消耗较大，这个没用过。



rootsignature就和cpp中的函数声明类似，是一种约定的行为（cpu与gpu之间的约定如何解释内存中的二进制数据）。

gpu再渲染的过程中，肯定需要各种各样的数据（纹理贴图，变换矩阵，法线，采样器信息等），而这些数据全是由cpu处理加载的，所以就需要有这么一种数据结构，用来描述这些数据。

也就是可以把gpu渲染看成是gpu调用的一个大函数，这些数据就是cpu向函数传递的参数。所以再执行这个函数之前，cpu必须和gpu之间约定好这些参数的类型，放在哪个寄存器中，参数的结构是怎样，这些就是rootsignature的作用。

需要注意的是rootsignature也需要序列化一下才能被gpu理解，也就是再创建rootsignture之前必须序列化。

```c++
// 就用这个序列化下就行应该
ComPtr<ID3DBlob> SerializeRootSignature;
ComPtr<ID3DBlob> errorSerializeRootSignature;
D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		SerializeRootSignature.GetAddressOf(),
		errorSerializeRootSignature.GetAddressOf());
```



rootsignature中也可以直接设置rootdescriptor以及静态采样器，这些被认为是静态的，也就是每一个rootsignature绑定一个，如果需要改变就需要设置rootsignature。

```c++
CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER::D3D12_FILTER_MIN_MAG_MIP_POINT)

// 这里就需要传递相应的根参数的数量，跟参数，静态采样器
CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		8, RootParam, StaticSamplerObject.GetSize(), StaticSamplerObject.GetData(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
```

