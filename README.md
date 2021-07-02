[TOC]


# SpaceTarget for Unreal Engine

## 前言
使用 **SpaceTarget Plugin** 与四维看看扫描的场景结合,可以帮助您创建虚实结合的丰富场景交互


<img src=doc/ueimgs/60ddcb1246dc4.gif width = "30%"  alt="tt0.top-832429.gif"/>

###开发环境设置

	Unreal Engine 4.26及以上版本

###运行环境

	Android 8.0 及以上版本
	iOS 11.0 及以上版本


##快速开始

### 1. 下载SpaceTarget插件
使用 [GitHub仓库地址: https://github.com/zhmxiaowo/SpaceTarget.git](https://github.com/zhmxiaowo/SpaceTarget.git "GitHub仓库地址: https://github.com/zhmxiaowo/SpaceTarget.git")

使用 [官网下载插件包](http://4dkankan.com/SpaceTarget.git "官网下载插件包")


### 2. 创建一个AR工程
- 启动引擎4.26,并创建 **Games -> HandheldAR** 

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/b57c603201861b00976999297d9eff88 width = "50%"  alt="image.png"/>

- 选择模板为 **HandheldAR** 

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/db22ce9a7061dce99e86590d83af357a width = "50%"  alt="image.png"/>

示例将项目起名为 **SPTest** ,然后创建项目

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/6823f5368b83c5b403cd3277bd872873 width = "50%"  alt="image.png"/>

### 3. 安装插件
- 在项目根目录下创建 **Plugins** 文件夹

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/4a2d120a309dcf978d1b74ad7111747a width = "50%"  alt="image.png"/>

- 将下载的 **SpaceTarget** 插件文件夹放入 **Plugins** 文件夹


<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/a147baceda61522908e115068f3bf5be width = "50%"  alt="image.png"/>

	Note:
	最终文件路径分布: .../SPTest/Plugins/SpaceTarget
---

- 双击项目文件夹重新打开工程,会提示rebuild,点击继续即可

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/257fab08db0279a75a8519912bc5e4ca width = "50%"  alt="image.png"/>

- 打开 **Settings -> Plugins** ,拖动左侧滑动条到底部,查看Others,此时可以看见SpaceTarget已经被加载进引擎
(如果Enabled未自动勾选,请手动勾选后重启引擎以启用SpaceTarget)

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/7b37aa6c7b39f7fe1ca461294dbd475b width = "50%"  alt="image.png"/>

### 4. 创建应用场景
- 我们将编辑默认场景 **HandheldBlankMap** .

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/134cb6587fcab4359cd1e49c7d0e3f50 width = "50%"  alt="image.png"/>

- 选择World Settings.并将 **GameMode** 设置为 **SpaceTargetGameMode** ,将 **Default Pawn Class** 设置为 **SpaceTargetPawn** 

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/0bdc70b9628706eca8da82f28a85219e width = "50%"  alt="image.png"/>

-  **SpaceTargetPawn** 中包含了AR定位的启动,如果您想使用自己的GameMode,只需要将 **SpaceTargetPawn** 中的蓝图拷贝至您的对应蓝图内


<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/9ea3f1863eee64f85d75ee8a65c32854 width = "50%"  alt="image.png"/>


- 在 **ContentBrowser** 右下角的 **View Options** 勾选上 **Show Plugin Content** 

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/95b90082f78aa24b44478e12bf29fa68 width = "50%"  alt="image.png"/>

- 选择 **SpaceTarget/Content** 文件夹

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/3766b08c2825e5e707e798dd2e5fe03e width = "50%"  alt="image.png"/>


- 在 **SpaceTarget Contents/SpaceTarget** 中并找到 **SpaceTargetActor** ,并将其拖放至场景中

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/b0dfea59473d02f3e777b63963ed8419 width = "50%"  alt="image.png"/>


- 选中刚刚放入场景的 **SpaceTargetActor** ,并在Details面板中找到DownloadData按钮,点击并下载四维看看拍摄的场景数据集
*如何获取数据?请参阅[数据获取](https://www.4dkankan.com "数据获取")*

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/e90332e63d2bf516357f2bd91d9b1710 width = "50%"  alt="image.png"/>

- 输入场景码,点击下载

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/56d8ec72d923a2e9828d218657fee60f width = "50%"  alt="image.png"/>

- 请耐心等待下载场景数据下载完成,完成后将会在项目文件夹下(SPTest)生成Content/SpaceTarget文件夹,内部包含下载场景的数据

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/788b8d927e9072842ee68c13bfb44dd8 width = "50%"  alt="image.png"/>

- 等待下载完成后,引擎会提示需要导入模型数据,点击Import

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/bef57d78771ecb9edf9c08489606d8a3 width = "50%"  alt="image.png"/>


- 将 **Import Uniform Scale** 设置为 **100** 后点击 **Import All**

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/5ce6e5a97af1d0642fb3109666d9d3b1 width = "50%"  alt="image.png"/>


-  **重新** 选择场景中的 **SpaceTargetActor** ,找到 **Data** 下拉框,选择刚刚导入的场景ID

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/6db338a9bf5719bd0bea6cc069cba2dc width = "50%"  alt="image.png"/>


此时可以看到我们的场景数据已经加载进来了.

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/1c43d200b91a0487c129de0ba0bfb17a width = "50%"  alt="image.png"/>

### 5. 自定义内容
- 在场景内放置内容,并将内容放置到 **SpaceTargetActor** 下作为子物体.示例放入了三个不同颜色的方块

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/e91fa044fc1310f1cef35dbe7b63d974 width = "50%"  alt="image.png"/>

- 这里我们选择隐藏掉场景模型的显示

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/7c061bb3d067f5089bf05e857bcfc041 width = "50%"  alt="image.png"/>

	Note:
	SpaceTargetActor的蓝图内包含了整个SpaceTarget的启动过程以及事件绑定过程
### 6. 打包场景
- 配置好打包设置后,即可到真实场景中还原3个立方体的位置.


<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/b72973e3c373026d4b6cb23e1ba03db0 width = "50%"  alt="image.png"/>


<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/9cea1bb50bfc563d52090919e1f38587 width = "50%"  alt="tt0.top-965488.gif"/>



如何配置打包场景?请参阅Epic Game官方文档 [iOS游戏开发](https://docs.unrealengine.com/4.26/zh-CN/SharingAndReleasing/Mobile/iOS/ "iOS游戏开发")    [Android游戏开发](https://docs.unrealengine.com/4.26/zh-CN/SharingAndReleasing/Mobile/Android/ "Android游戏开发")

### 7. 自定义开发内容补充
- 我们提供了两个委托代理,可以绑定当场景被识别与丢失的回调

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/4b6983cbfd1ce35e9aced85da3bc6773 width = "50%"  alt="image.png"/>


	Note:
	示例中的SpaceTargetActor绑定了
	1.当场景被识别,显示SpaceTargetActor以及其子物体
	2.当场景丢失,隐藏SpaceTargetActor以及其子物体


- SpaceTargetActor功能详情

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/31b9674cb07c1cd557ef15887a05a605 width = "50%"  alt="image.png"/>

| 属性  |  用途 |
| ------------ | ------------ |
| Data  | 场景列表选择框,重新选择Actor刷新此表,用于标记当前使用的场景数据  |
|  Download | 下载按钮,点击即可使用下载界面下载数据  |
| Data Base  | 内部用属性,随着Data改变  |
|  Center Mode | Target: SpaceTargetActor场景数据静止,改变Camera匹配相对位位置. Device: SpaceTargetActor随着相机改变相对位置  |
| Visible Model | 显示与隐藏当前场景Mesh,编辑使用True,发布通常使用False |

### 8. 如何使用自己的AR定位系统

此项涉及代码与工程的重构,建议备份好当前数据

	Note:
	使用自己的AR定位系统,需要继承ISlamGenerator并重写相应方法,详情可参考
	Project/Plugins/SpaceTarget/Source/SpaceTarget/Private/ARKitSlamGenerator.h
	Project/Plugins/SpaceTarget/Source/SpaceTarget/Private/ARCoreSlamGenerator.h

示例代码:

```
UCLASS(BlueprintType, Blueprintable, Category = "4DMR-SpaceTarget")
class AARKitSlamGenerator : public AActor, public ISlamGenerator
{
	GENERATED_BODY()
	......
public:
	/*get slam current camera intrinsics interface,you need to overwrite it.*/
	virtual bool GetCameraIntrinsics(FCameraIntrisics& ci) override;
	/*get slam current pose interface,you need to overwrite it.*/
	virtual bool GetCameraPose(FCameraPose& cp) override;
	/*set slam current pose interface*/
	virtual void SetCameraPose(FTransform cp) override;
	/*get slam tracking state.you need to overwrite it.*/
	virtual bool GetTrackingStatus(FXRTrackingState& ts) override;
	/*get slam tracking texture which type is RGBA8888,you need to overwrite it*/
	virtual bool GetCameraTexture(FCameraTexture& ct) override;
	......
}
```

- 移除相关ARKit,ARCore的内容
1.删除 **.../Plugins/SpaceTarget/SpaceTarget.uplugin** 内对ARKit与ARCore的插件引用,同时确保项目的(SPTest.uplugin)没有此引用

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/56048abdca6e1c404ecba2b662b639ba width = "50%"  alt="image.png"/>

2.删除 **.../Plugins/SpaceTarget/Source/SpaceTarget/SpaceTarget.Build.cs** 对ARKit与ARCore插件的引用

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/f28aafdfafee61d214d86094bd8d1d30 width = "50%"  alt="image.png"/>

3.删除调用文件
删除 **.../Plugins/SpaceTarget/Source/SpaceTarget/Public/** 目录以下文件:

 **ARCoreSlamGenerator.h**

 **ARKitSlamGenerator.h**

删除 **.../Plugins/SpaceTarget/Source/SpaceTarget/Private/** 目录以下文件:

 **ARCoreSlamGenerator.cpp**

 **ARKitSlamGenerator.cpp**

4.修改SpaceTarget Content/SpaceTargetActor蓝图内容,将slam修改为自定义继承自ISlamGenearator的类

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/70fb2ca2fba55e8cbf5485f20174adda width = "50%"  alt="image.png"/>

完成后,右键项目启动文件,重新Generate一次项目即可使用自定义的AR定位

## FAQ

###打包后无法正常使用?
我们建议您使用C++构建工程,构建C++工程的方式是通过 **File -> New C++ Class** 创建一个简单的c++类.

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/124f476e9853152b292b1944235e51b4 width = "50%"  alt="image.png"/>

然后重新 Generate 工程并编译项目,即可正常使用.

###如何快速进行Android打包测试?
- 点击Configure Now

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/e846d6b9254100f1acbc719db1dae82a width = "50%"  alt="image.png"/>

- 点击Configure Now并,建议取消勾选Include AdMob support for ads.

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/59f923f348f642740ec7b1467e6bd9db width = "50%"  alt="image.png"/>

- 选择打包支持arm64版本的数据包

<img src=http://doc.4dage.com:2999/server/index.php?s=/api/attachment/visitFile/sign/f829c114b1b2e3a0b522890c1cd4219c width = "50%"  alt="image.png"/>


选择File -> Package Project -> Android -> Android(ASTC),选择一个文件夹即可导出.apk
