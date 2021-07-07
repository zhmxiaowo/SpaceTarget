# 如何使用自己的AR定位系统

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

<img src=doc/ueimgs/60dd34b02b3e4.png width = "50%"  alt="image.png"/>

2.删除 **.../Plugins/SpaceTarget/Source/SpaceTarget/SpaceTarget.Build.cs** 对ARKit与ARCore插件的引用

<img src=doc/ueimgs/60dd353eefce5.png width = "50%"  alt="image.png"/>

3.删除调用文件
删除 **.../Plugins/SpaceTarget/Source/SpaceTarget/Public/** 目录以下文件:

 **ARCoreSlamGenerator.h**

 **ARKitSlamGenerator.h**

删除 **.../Plugins/SpaceTarget/Source/SpaceTarget/Private/** 目录以下文件:

 **ARCoreSlamGenerator.cpp**

 **ARKitSlamGenerator.cpp**

4.修改SpaceTarget Content/SpaceTargetActor蓝图内容,将slam修改为自定义继承自ISlamGenearator的类

<img src=doc/ueimgs/60dd374144a58.png width = "50%"  alt="image.png"/>


5.删除 **SpaceTargetPawn** 蓝图中定义 **Start AR Session** ,改用您的AR启动函数

完成后,右键项目启动文件,重新Generate一次项目即可使用自定义的AR定位