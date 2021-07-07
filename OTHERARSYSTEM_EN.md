# How to use your own AR system(for Location)

This action involves refactoring of code and engineering, it is recommended to back up the current data 

	Note:
	To use your own AR system, you need to implement ISlamGenerator and overwrite the virtual method. For details, please refer:
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

- Remove related ARKit,ARCore

1.Delete the plugin reference to arkit and arcore in **.../Plugins/SpaceTarget/SpaceTarget.uplugin**, and ensure that the (SPTest.uplugin) project does not have this reference

<img src=doc/ueimgs/60dd34b02b3e4.png width = "50%"  alt="image.png"/>

2.Delete the reference **.../Plugins/SpaceTarget/Source/SpaceTarget/SpaceTarget.Build.cs** about ARKitand ARCore

<img src=doc/ueimgs/60dd353eefce5.png width = "50%"  alt="image.png"/>

3.Delete file
Delete files in **.../Plugins/SpaceTarget/Source/SpaceTarget/Public/** 

 **ARCoreSlamGenerator.h**

 **ARKitSlamGenerator.h**

Delete files in **.../Plugins/SpaceTarget/Source/SpaceTarget/Private/** 

 **ARCoreSlamGenerator.cpp**

 **ARKitSlamGenerator.cpp**

4.Modify the content of the SpaceTarget Content/SpaceTargetActor blueprint and modify the slam to be custom implement from ISlamGenearator 

<img src=doc/ueimgs/60dd374144a58.png width = "50%"  alt="image.png"/>

5.Change **Start AR Session** on **SpaceTargetPawn** blueprint to your AR start method.

After completion, right-click the project startup file and regenerate the project to use the customized AR System