# GameEngine
 directx12和游戏引擎学习

视频演示：

基础光照模型：
![光照模型_](https://user-images.githubusercontent.com/62169763/170821706-abfcbcaa-9747-4294-8631-a3c3cad034c7.gif)
纹理贴图->法线贴图->高光贴图
![贴图](https://user-images.githubusercontent.com/62169763/175771694-0483f5ac-b08a-47ad-b7f1-9a7eff15ce1f.gif)
雾，天空盒，透明
![雾，透明，天空盒](https://user-images.githubusercontent.com/62169763/182022487-53f2bc59-0e67-47ca-963d-c449aadd8dff.gif)

# 1 DX12的一些小概念

##  CommandAllocator  GraphicsCommandList  CommandQueue

1 我们可以向命令列表中添加命令，由cpu将命令列表中的命令提交到命令队列中，gpu就会按照命令队列的顺序依次执行命令，**需要注意的是当命令队列满的时候，cpu就无法在继续提交处于空闲状态，而当命令队列空的时候，gpu就会处于空闲状态**，，

2 下一个概念就是命令分配器，，，他是与命令列表相关联，存储在命令列表中的命令实际上是存储到了命令分配器中。**我们可以创建出多个命令列表关联一个命令分配器，但命令列表不能同时存储命令。**原因是：需要保证命令列表中的数据按照顺序添加到命令分配器中，所以与关联相同命令分配器的命令列表，其中只能有一个命令列表有命令。

3 我们在将命令列表中的命令添加到命令队列中后，就可以执行命令列表.Reset()方法，清空命令列表，就可以复用这个命令列表来存储其他命令。

但是不能将命令分配器清空，因为在将命令提交给命令队列后，命令队列有可能会引用命令分配器中的数据，再加上gpu并不一定会立即执行，所以必须保证gpu在执行完名列队列中的命令后，再将命令分配器清空。

### 为什么要有commandallocator呢？

gpu通过执行命令来实现渲染的操作，而这些命令是通过cpu传过来的。也就是说cpu需要将命令列表中的命令放到显存中 ，而gpu需要从显存中读取命令在执行，这样就对显存有了一读一取的操作，，，在多线程的情况下就会有同步的问题，所以就有了commandallocator来管理显存这块存放命令的区域。这就好比我在饭店点菜，服务员将我点的菜记录到纸上，点完之后，服务员将菜单递交给后厨，后厨根据每道菜依次做，这里的纸就好比是commandallocator。

## D3dDevice

驱动可做的事情很多：

可以帮助我们来创建一些东西：CreateCommandList，CreateCommandQueue，CreateCommandAllocator，CreateFence，CreateDepthStencilView

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

## rootsignature

rootsignature就和cpp中的函数声明类似，是一种约定的行为（cpu与gpu之间的约定如何解释内存中的二进制数据）。

gpu再渲染的过程中，肯定需要各种各样的数据（纹理贴图，变换矩阵，法线，采样器信息等），而这些数据全是由cpu处理加载的，所以就需要有这么一种数据结构，用来描述这些数据。

也就是可以把gpu渲染看成是gpu调用的一个大函数，这些数据就是cpu向函数传递的参数。所以再执行这个函数之前，cpu必须和gpu之间约定好这些参数的类型，放在哪个寄存器中，参数的结构是怎样，这些就是rootsignature的作用。

> 需要注意的是rootsignature也需要序列化一下才能被gpu理解，也就是再创建rootsignture之前必须序列化。
>
> rootsignature中也可以直接设置rootdescriptor以及静态采样器，这些被认为是静态的，也就是每一个rootsignature绑定一个，如果需要改变就需要设置rootsignature。

# 2 渲染流水线

1 图元装配阶段：需要将传递进来的顶点数据，索引数据装配成一个个的三角形图元

2 顶点着色器阶段：

​	（model transformation）将局部空间的每个顶点，经过线性变换和平移变换到世界空间的位置上

​	（view transformation）将世界空间的坐标点变换到摄像机空间，，，摄像机变换矩阵：**3d中变换顺序为缩放->旋转->平移**，，，所以矩阵

[平移矩阵] * [旋转矩阵] * [坐标点]，，，但是view transformation是基变换，，所以需要求逆，，变成了[旋转矩阵]-1 * [平移矩阵]-1 * [坐标点]，

其中旋转矩阵是正交矩阵，逆就是转置，平移矩阵的逆就加个负号。

​	（projection transformation）投影变换

​				（orth projection）正交投影，将一个**视矩体转变换到一个中心点在原点的正方体**，此处依然**不采用3d中的变换顺序**，因为视矩体要变成正方体的缩放轴是以正方体原点为中心的，所以需要先将视矩体平移到摄像机空间下的原点，然后在进行缩放操作，也就是

[缩放矩阵] * [平移矩阵] * [坐标点]，无论是缩放还是平移都需要获取到视矩体的各项参数（near plane, far plane, top plane, bottom plane, left plane, right plane）,这些参数主要是通过（near plane, far plane, fov, width/height）求得。

平移就是将视矩体的中心点平移到原点，而缩放就是将位于原点的视矩体的[x,y,z轴]都缩放为2个单位长度。

​				（pers projection）透视投影，模拟实际情况，近大远小的效果，同样的将**视锥体变换到一个中心点在原点的正方体**，此处的方案**首先将视锥体变换成视矩体，在左乘正交投影矩阵**。此出所需要的参数是（ near plane, far plane），此出需要注意的是矩阵的第三行求法，用远近平面的z值，做特殊值代入法。

在左乘投影矩阵后，此时顶点会位于正方体中，也就是齐次剪裁空间，在这里我们可以保存每个点的w分量（表示每个点在世界空间下的深度值），然后在进行透视除法。

> 对平移矩阵的理解：我们所说的线性变换都是基于原点不变的情况，所以平移不是线性变换，他需要移动原点（这也是三维世界中变换顺序为缩放->旋转->平移的原因）。所以我们就用更高维度的矩阵来表示，拿二维点的平移举例，平移矩阵所构成的列空间是三维的，其中ox,oy都是标准正交基，而oz轴是假设为（dx,dy,1），我们在左乘平移矩阵后，就是将二维点的z分量也就是z方向上的单位向量，变换到列空间中oz轴上的单位向量，左乘完成后，在三维空间中的点，在xoy面上的投影点就是平移后点的位置。

> 对透视除法的理解：我们的投影操作是指把视锥体转换成位于原点的立方体，没有经过维度的变换，所以**投影矩阵应该是满秩的**，投影的过程中包括平移，所以**投影矩阵应该是四维的**，顶点在左乘投影矩阵后，会变换到四维空间中，四维空间中的w分量，决定了三维模型的放大和缩小，所以w应该一直保持为1，即透视除法。（可以类比三维空间中的投影仪，投影仪在墙上投影出一幅2d画面，在三维空间中w分量表示，投影仪与墙的距离，w增大->投影仪与墙的距离增大->2d画面越大，反之同理，所以三维空间中w分量控制了2d画面的放大和缩小，推导回四维空间中就可理解）。

3 曲面细分阶段：能够将网格中的三角形再细分成很多三角形，从而有高模，低模之分，即lod的实现。

4 几何着色器阶段：

5 剪裁：我们是在正方体空间做剪裁。在左乘透视投影之后透视除法之前执行剪裁，用遍历六个面对三角形进行剪裁（这里就有两种情况：剪裁出的是四边形或者是三角形）。

6 光栅化阶段：

​	视口变换，将经历剪裁后的顶点数据左乘视口变换矩阵（先缩放->平移）

​	背面剪裁，通过每个三角形的卷绕方向，通过三角形的法向量是否大于0，来决定此三角形是正向还是反向。

​	重心坐标插值来给每个像素上颜色：通过重心坐标来插值计算出三角形中人一顶点的各种数据（颜色，贴图位置）。

7 输出合并阶段，

# 3 基础光照模型

> shader里面颜色计算，这边简单理解下，有时候加乘的
>
> 1 加
>
> 看文章说是与三原色和三基色有关，三原色一般指的是红，绿，蓝，简称RGB，这是加色系。就是光源只含有特定的波段，本身就是色光，将不同的光加在一起形成新的颜色。例子就是显示屏。颜色相加的多了就会接近1，也就是白色了，所以再算的时候都是环境光，高光，漫反射都是相加的。
>
> 2 乘
>
> 这部分应该就是涉及三基色，一般指的是颜料三原色，在纯白光照射下颜色为绛红，黄，青，简称CMYK，属于减色系。他们本身不发光，靠反光被看到，由于材料吸收特定波段的光，所以只有不被吸收的部分反射回来，也就是咱们看到的颜色。所以在算材质受到光照影响的时候都是用乘的
>
> 3 减
>
> 能实现颜色的反相，白变黑，黑变白。

光照方向 L

顶点法线 N

视角方向 V

## 兰伯特

```hlsl
        Ambient = Material.BaseColor * AmbientLight;//环境光
        diffuse = Material.BaseColor * max(LdotN, 0.f);//漫反射
```

## 半兰伯特

```
        Ambient = Material.BaseColor * AmbientLight;	//环境光
        diffuse = Material.BaseColor * (LdotN * 0.5f + 0.5f);//漫反射
```

兰伯特的改进项，用兰伯特渲染出的材质球，光照背面近乎死黑。

是因为余弦取值为[-1, 1]，而max修改范围为[0, 1]，从而使光照背面全为0也就是黑色。

半兰伯特就是将[-1, 1]的范围，映射到[0, 1]，材质球的范围是从0到1，所以整个材质球都有颜色。

## phone

```
        Ambient = Material.BaseColor * AmbientLight;				//环境光
        diffuse = Material.BaseColor * (LdotN * 0.5f + 0.5f);		//半兰伯特
        //float3 reflectDirection = 2.0f * LdotN * N - L;			//光线反射光线
        float3 reflectDirection = reflect(-L, N);					//光线反射光线
        float MaterialShininess = 1.f - saturate(Roughness);		//粗糙度
        float M = max(MaterialShininess * 100.f, 1.0f);				//次方
        specular = Material.BaseColor * pow(max(dot(normalize(reflectDirection), V), 0.f), M);	//高光
```

模拟金属材质：视线在观察金属的时候，会出现光斑。

通过反射光线 与 视线 的夹角来计算

## blinn-pbone

```
        Ambient = Material.BaseColor * AmbientLight;				//环境光
        diffuse = Material.BaseColor * (LdotN * 0.5f + 0.5f);		//半兰伯特
        float3 H = L + V;											//摄像机方向和光线入射方向的半程向量			
        float MaterialShininess = 1.f - saturate(Roughness);		//粗糙度
        float M = max(MaterialShininess * 100.f, 1.0f);				//次方
        specular = Material.BaseColor * pow(saturate(dot(N, normalize(H))), M);//高光
```

通过计算半程向量简化了反射光线的计算，提高了效率。

通过 半程向量（观察方向和入射光线的和）与 法线的夹角来计算。

## wrapLight

```
        float w = 3.f;										//w为0 是兰伯特材质，w为1 是半兰伯特
        Ambient = Material.BaseColor * AmbientLight;		//环境光
        diffuse = Material.BaseColor * saturate(((LdotN + w) / (1.f + w)));//漫反射
```

基本上可以说是半兰伯特的高级版。

主要用来模拟粗糙表面，类似于皮肤的粗糙表面，能够通过系数调整粗糙程度。

将原本的cos范围映射到相应的范围中，以体现粗超程度。

### Minnaert Lighting

```
        Ambient = Material.BaseColor * AmbientLight;
        float NdotL = dot(N, L);
        float VdotN = dot(V, N);
        
        /*简单实现dot * dot*/
        //diffuse = Material.BaseColor * saturate(NdotL) * saturate(VdotN);
        
        /*有粗糙度的实现*/
        float MaterialShininess = saturate(Roughness);
        diffuse = Material.BaseColor * saturate(NdotL) * pow(saturate(NdotL) * saturate(VdotN), MaterialShininess);
```

适合多孔渲染，类似于月亮，天鹅绒，丝袜。

通过光线法线点击 * 视线法线点击 来计算。

## BandedLight

```
        Ambient = Material.BaseColor * AmbientLight;//环境光
        float UpDotValue = (LdotN + 1.0f) * 0.5f;	//半兰伯特
        float layout = 4.0f;						//分层数量
        UpDotValue = floor(UpDotValue * layout) / layout;
        diffuse = Material.BaseColor * UpDotValue;
```

形成光带，用于卡通渲染居多。

通过上取整的方式，将[0, x]的范围，分割成x个部分，每个部分的颜色都一样。

## 次表面散射sss

```
        //环境光
        Ambient = Material.BaseColor * AmbientLight;
        
        //漫反射
        float w = 1.2f; //w为0 是兰伯特材质，w为1 是半兰伯特
        diffuse = Material.BaseColor * saturate(((LdotN + w) / (1.f + w)));
        
        //高光
        float3 reflectDirection = reflect(-L, N);
        float MaterialShininess = 1.f - saturate(Roughness);
        float M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(max(dot(normalize(reflectDirection), V), 0.f), M);
        
        //次表面散射
        float sssValue = 1.3f;
        float TransmissionIntensity = 2.f;
        float TransmissionScope = 1.5f;
        float3 Half = -normalize(L + N * sssValue);		//求得半程向量
        LdotN = pow(saturate(dot(V, Half)), TransmissionScope) * TransmissionIntensity;//背光强度
        diffuse = diffuse + Material.BaseColor * LdotN;
```

具有透光的效果，模拟玉石。

其中背光比例是由，视线和 负的半程向量（入射光线 和 法线 和）点乘得到

# 4 法线贴图和切线空间

1. 法线的空间变换

   我们在进行光照计算的时候，就会用到法线，这里我们也需要将模型空间下的法线转换到世界空间下，模型顶点的转换就用顶点坐标乘上obiect2world矩阵就行，但是法线就需要乘上object2world的逆转置矩阵，这是为什么？

   ​	回答1：形象理解上，法线是垂直于切线的向量，当切线以不成比例缩放的时候，就会影响法线的方向，法线如果按照相同变换那么其将不会与平面继续保持垂直。法线这时候就需要按照反方向变换，也就是需要object2world的逆。object2world矩阵中包含缩放，旋转，平移的信息，法线是向量所以将平移舍去变为（3X3矩阵），所以我们求逆的时候得到了缩放，旋转矩阵的逆，这时候得到的矩阵里面旋转也变成了逆，不是我们想要的（因为旋转不会影响法线方向），旋转矩阵又为正交矩阵（转置等于逆），所以很自然的转置就得到了缩放矩阵的逆转置和旋转矩阵。也就是world2object矩阵中包含了缩放矩阵的逆转置和旋转矩阵，也就是当不是等比例缩放的时候，我们的法线需要乘object2world的逆转置矩阵。

   ​	回答2：不严谨的数学计算上，法线是垂直于切线的向量，也就是 n * t = 0 ，法线点乘切线等于0，我们模拟切线乘上矩阵A，法线乘上矩阵B，公式就变成了

   Bn * At = 0。乘号两边同乘矩阵A的逆得A-1Bn * t = 0，要想等式成立，即A-1B = 1(单位向量)，也就是矩阵A是矩阵B得逆变换，也就是法线在进行空间变换得时候需要乘上切线变换矩阵得逆变换矩阵。

2. 什么是法线贴图呢，贴图里面存储了什么？

   1 我们渲染一个模型，可以给每个顶点法向量来让其进行光照等计算，但是模型顶点数量的增多，也会导致计算压力过大，所以我们就可以仿照纹理贴图的形式，搞一个法线贴图来存储每个采样点得法线偏移，通过每个点得实际法线吗，在添加上法线贴图的偏移，就是最终用于计算光照等信息的法线。法线贴图也是用rgb三种颜色来代替法向xyz，如果rgb颜色值直接与法向量一一对应，那么模型只要改变，或者说是模型与贴图不匹配，计算出的法向量就会有问题，并且立方体有六个面就需要六张法线贴图。所以为了通用性，我们将从rgb中读取的颜色值代表法线偏移放在切线空间，在通过tbn矩阵将其变换到世界空间下，就能准确得出，此时采样点的法向量。

   2 我们拿一个墙面举例，如果我们只用纹理贴图，那么这个墙面看起来就会很光滑没有那种凹凸的表现，而凹凸表现一般都是通过光照来体现，进一步就是法线影响的，我们在创建这个墙面的时候会赋予每个三角形网格顶点法线，然后三角形网格中的法线都是通过插值来得到，因为这是个墙面也就是个平面所以法线都是一个值，自然就不会有那种凹凸的感觉。所以我们在插值得到法线之后，在基于这个法线赋予其一个偏移不就可以改变法线进而得到凹凸的效果，而这个偏移我们就存储在了贴图中，也就是法线贴图。

3. 什么是切线空间呢？

   一个空间就是要找到组成这个空间的3个基向量，切线空间是位于三角形表面之上的空间，xyz轴分别为t轴（切线方向）,b轴（负切线方向），n轴（法线方向）,设定切线空间的z轴和法线同向，那么x,y轴呢？曲面上的某点切线有无数条，共同组成了切平面，所以规定了一套固定的x,y轴，也就是uv展开方向相同的那个方向作为切线方向（屏幕左下角原点，向右是切线方向，向上是负切线方向）。

4. 为什么要有切线空间，切线空间好处？

   1 模型空间：

   优点：好理解，简单直观

   缺点：如果模型顶点发生改变，那么贴图也要随之改变

   2 切线空间

   优点：

   此空间下存储的是相对法线信息，因此换个网格应用该纹理，也能得到相应的效果。

   可以进行uv动画，通过移动该纹理的uv坐标实现凹凸移动的效果。

   可以复用，相同模型都可以用这一张纹理

   可以压缩，贴图中存储的都是归一化后的值，所以就可以只存储xy方向，从而推导出z方向（三维归一化之后点组成的空间就是单位球，xy组成的是平面，平面和球有四个交点，有两个是xy反方向交点舍去，再把z为负的交点舍去，自然就剩下一个交点也就是z值了）

5. 为什么法线贴图偏蓝呢？

   贴图中存储的都是原法线的偏移量，并且用归一化后的向量表示，也就是范围是[-1.1]而贴图中rgb颜色能够表示的范围是[0,1]，所以需要进行一个( x + 1) / 2的转换。当法线没有偏移的时候取值2为（0，0，1）转换后贴图中存储的是（0.5，0.5，1）也就是偏蓝色了。

6. TBN矩阵

   TBN矩阵定义了一个点从切线空间变换到世界空间或者模型空间的变换矩阵：当N是模型空间中的法线时，计算出的TBN是切线空间转模型空间矩阵，若N是世界空间中的法线时，计算出的TBN是切线空间转世界空间矩阵。

   我处理的方式就是，通过将模型的顶点数据（顶点位置，顶点法线，uv坐标，切线方向）传给顶点着色器，通过转到世界空间和T X N = B，进而得到TBN矩阵，传到片元着色器，然后 TBN X Normal得到最终的法线进而计算光照等信息。

   需要注意的是：TBN矩阵中的N是原法线（也就是模型顶点的真实法线），而Normal是从法线贴图中提取出来然后经过转换的法线，相乘得出的才是真正应该计算光照等信息的法线。

7. 具体如何实现呢？

   每个顶点结构需要在位置，法向（此时的法向不用来计算光照，只是用来计算tbn矩阵）的基础上增加一个切向量t(tangent)，也就是用来计算tbn矩阵。

   切线空间是由法向（Z）、Tangent（切线t）、bitangent（切线b）三个向量组成。我们在顶点着色器中获取Z和t以及b(z和t叉乘)来构建tbn矩阵，再将tbn矩阵传入片元着色器。

   在片元着色器中采样获取到法线贴图中的rgb颜色（贴图中的范围是[0, 1]，而切线空间的范围是[-1, 1]）然后进行映射x * 2 - 1 获取到切线空间中的法向量，然	然后将其乘tbn矩阵，也就是从切线空间转换到了世界空间，以便后续光照等信息的计算。

# 5 渲染反射

层划分：

背景 -> 不透明物体-> 透明物体-> 不透明物体反射层->背景->不透明物体->透明物体

基本流程：

1 在反射球的6个面上创建6个摄像机

2 在创建6个rtv，根据6个摄像机的位置绘制6遍场景，将这6遍场景赋值到6个rtv中

3 在设置交换链上的主rtv，根据主摄像机绘制反射物体，注意这里要把上面6个rtv当成srv（也就是cubemap贴图资源）传到shader里面

这样就实现了反射球的渲染，要渲染多遍场景只为了渲染一个带有反射的物体，很费

关键类：FDynamicCubeMap（），FCubeMapRenderTarget
