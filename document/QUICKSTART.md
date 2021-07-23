[TOC]


# 快速开始


### 1. 下载SpaceTarget插件
使用 [GitHub仓库地址: https://github.com/zhmxiaowo/SpaceTarget.git](https://github.com/zhmxiaowo/SpaceTarget.git "GitHub仓库地址: https://github.com/zhmxiaowo/SpaceTarget.git")

使用 [官网下载插件包](https://www.4dkankan.com/#/developer/downSDK "官网下载插件包")


### 2. 创建一个AR工程
- 启动Unreal Engine引擎,并创建 **Games -> HandheldAR** ,示例将项目起名为 **SPTest** ,然后创建项目

<img src=doc/ueimgs/60dc2dd136420.png width = "50%"  alt="image.png"/>

### 3. 安装插件
- 在项目根目录下创建 **Plugins** 文件夹,将下载的 **SpaceTarget** 插件文件夹放入 **Plugins** 文件夹


<img src=doc/ueimgs/60dc37c2cda74.png width = "50%"  alt="image.png"/>

	Note:
	最终文件路径分布: .../SPTest/Plugins/SpaceTarget
---

- 双击项目文件夹重新打开工程,会提示rebuild,点击继续即可

<img src=doc/ueimgs/60dc3892df25c.png width = "50%"  alt="image.png"/>

- 打开 **Settings -> Plugins** ,拖动左侧滑动条到底部,查看Others,此时可以看见SpaceTarget已经被加载进引擎
(如果Enabled未自动勾选,请手动勾选后重启引擎以启用SpaceTarget)

<img src=doc/ueimgs/60dc3b73de209.png width = "50%"  alt="image.png"/>

### 4. 创建应用场景
- 我们将编辑默认场景 **HandheldBlankMap** .

<img src=doc/ueimgs/60dc476562928.png width = "50%"  alt="image.png"/>

- 选择World Settings.并将 **GameMode** 设置为 **SpaceTargetGameMode** ,将 **Default Pawn Class** 设置为 **SpaceTargetPawn** 

<img src=doc/ueimgs/60dc48295b29a.png width = "50%"  alt="image.png"/>

-  **SpaceTargetPawn** 中包含了AR定位的启动,如果您想使用自己的GameMode,只需要将 **SpaceTargetPawn** 中的蓝图拷贝至您的对应蓝图内, **SpaceTargetPawn** 含有描述ARKit ARCore位置的Camera组件.

<img src=doc/ueimgs/60dd248ca974c.png width = "50%"  alt="image.png"/>

- 在 **SpaceTarget Contents/SpaceTarget** 中并找到 **SpaceTargetActor** ,并将其拖放至场景中

<img src=doc/ueimgs/60dc4c8d86a69.png width = "50%"  alt="image.png"/>


- 选中刚刚放入场景的 **SpaceTargetActor** ,并在Details面板中找到DownloadData按钮,点击并下载四维看看拍摄的场景数据集
*如何获取数据?请参阅[数据获取](https://www.4dkankan.com/#/developer/useCollection "数据获取")*

<img src=doc/ueimgs/60dc42a1bf0e0.png width = "50%"  alt="image.png"/>

- 输入场景码,点击下载

<img src=doc/ueimgs/60dc42d754bba.png width = "50%"  alt="image.png"/>

- 请耐心等待下载场景数据下载完成,引擎如果提示导入消息,请忽略即可,完成后将会在项目文件夹下(SPTest)生成Content/SpaceTarget文件夹,内部包含下载场景的数据

<img src=doc/ueimgs/60ddcb1246dc5.png width = "50%"  alt="image.png"/>


-  **重新** 选择场景中的 **SpaceTargetActor** ,找到 **Data** 下拉框,选择刚刚导入的场景ID

<img src=doc/ueimgs/60dc4724a929d.png width = "50%"  alt="image.png"/>


此时可以看到我们的场景数据已经加载进来了.

<img src=doc/ueimgs/60dc46dc82930.png width = "50%"  alt="image.png"/>

### 5. 自定义内容
- 在场景内放置内容,并将内容放置到 **SpaceTargetActor** 下作为子物体.示例放入了三个不同颜色的方块

<img src=doc/ueimgs/60dc4a221ffd3.png width = "50%"  alt="image.png"/>

- 这里我们选择隐藏掉场景模型的显示

<img src=doc/ueimgs/60dc4aaf799d0.png width = "50%"  alt="image.png"/>

	Note:
	SpaceTargetActor的蓝图内包含了整个SpaceTarget的启动过程以及事件绑定过程
### 6. 打包场景
- 配置好打包设置后,即可到真实场景中还原3个立方体的位置.


<img src=doc/ueimgs/60dd7e7674168.png width = "50%"  alt="image.png"/>


<img src=doc/ueimgs/60dd7ff416e2b.gif width = "30%"  alt="tt0.top-965488.gif"/>



如何配置打包场景?请参阅Epic Game官方文档 [iOS游戏开发](https://docs.unrealengine.com/4.26/zh-CN/SharingAndReleasing/Mobile/iOS/ "iOS游戏开发")    [Android游戏开发](https://docs.unrealengine.com/4.26/zh-CN/SharingAndReleasing/Mobile/Android/ "Android游戏开发")

### 7. 自定义开发内容补充
- 我们提供了两个委托代理,可以绑定当场景被识别与丢失的回调

<img src=doc/ueimgs/60dd2c96b835a.png width = "50%"  alt="image.png"/>


	Note:
	示例中的SpaceTargetActor绑定了
	1.当场景被识别,显示SpaceTargetActor以及其子物体
	2.当场景丢失,隐藏SpaceTargetActor以及其子物体


- SpaceTargetActor功能详情

<img src=doc/ueimgs/60ddc78ce2ceb.png width = "50%"  alt="image.png"/>

| 属性  |  用途 |
| ------------ | ------------ |
| Data  | 场景列表选择框,重新选择Actor刷新此表,用于标记当前使用的场景数据  |
|  Download | 下载按钮,点击即可使用下载界面下载数据  |
| Data Base  | 内部用属性,随着Data改变  |
|  Center Mode | Target: SpaceTargetActor场景数据静止,改变Camera匹配相对位位置. Device: SpaceTargetActor随着相机改变相对位置  |
| Visible Model | 显示与隐藏当前场景Mesh,编辑使用True,发布通常使用False |



