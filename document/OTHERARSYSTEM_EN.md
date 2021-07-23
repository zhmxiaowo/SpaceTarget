# How to use your own AR system(for Location)

This action involves refactoring of code and engineering, it is recommended to back up the current data 

	Note:
	To use your own AR system, you need to implement ISlamGenerator and overwrite the virtual method. For details, please refer:
	Project/Plugins/SpaceTarget/Source/SpaceTarget/Private/ARKitSlamGenerator.h
	Project/Plugins/SpaceTarget/Source/SpaceTarget/Private/ARCoreSlamGenerator.h

Example:

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

1.Delete the plugin reference to arkit and arcore in **.../Plugins/SpaceTarget/SpaceTarget.uplugin** , and ensure that the (SPTest.uplugin) project does not have this reference

<img src=doc/ueimgs/60dd34b02b3e4.png width = "50%"  alt="image.png"/>

2.Delete the reference  ** .../Plugins/SpaceTarget/Source/SpaceTarget/SpaceTarget.Build.cs **  about ARKitand ARCore

<img src=doc/ueimgs/60dd353eefce5.png width = "50%"  alt="image.png"/>

3.Delete file
Delete files in ** .../Plugins/SpaceTarget/Source/SpaceTarget/Public/ ** 

 **ARCoreSlamGenerator.h**

 **ARKitSlamGenerator.h**

Delete files in  **.../Plugins/SpaceTarget/Source/SpaceTarget/Private/**  

 **ARCoreSlamGenerator.cpp**

 **ARKitSlamGenerator.cpp**

4.Modify the content of the SpaceTarget Content/SpaceTargetActor blueprint and modify the slam to be custom implement from ISlamGenearator 

<img src=doc/ueimgs/60dd374144a58.png width = "50%"  alt="image.png"/>

5.Change **Start AR Session** on **SpaceTargetPawn** blueprint to your AR start method.

After completion, right-click the project startup file and regenerate the project to use the customized AR System

---

#### Image coordinate correction

The different AR system has a different coordinate,we need to set the **FCameraTexture.direction** to mapping the spacetarget coordinate.

```
enum class FImageDirection : uint8
{
	
	LEFT = 2,
	RIGHT = 3,
	UP = 0,
	DOWN = 1
};
```

- normal AR view:

<img src=doc/ueimgs/ue4.jpg   alt="image.jpg"/>


- example:ARKit & ARCore view:


<img src=doc/ueimgs/arkitarcore.jpg   alt="image.jpg"/>


Take arkit arcore as an example: the image is rotated -90 degrees to the left, so it is set to:

```
 ct.direction = FImageDirection::LEFT;
 
```


- In SpaceTarget, we set the orientation of the image as:


<img src=doc/ueimgs/spacetarget.jpg  alt="image.jpg"/>



In order to get the correct orientation relationship

```
void USpaceTargetSubsystem::FixImageAixs(FImageDirection dir, ImageAixs& tempaix)
{
	switch (dir)
	{
	case FImageDirection::LEFT:
		tempaix.X = FString("-z");
		tempaix.Y = FString("-y");
		tempaix.Z = FString("x");
		break;
	case FImageDirection::RIGHT:
		tempaix.X = FString("z");
		tempaix.Y = FString("y");
		tempaix.Z = FString("x");
		break;
	case FImageDirection::UP:
		tempaix.X = FString("y");
		tempaix.Y = FString("-z");
		tempaix.Z = FString("x");
		break;
	case FImageDirection::DOWN:
		tempaix.X = FString("-y");
		tempaix.Y = FString("z");
		tempaix.Z = FString("x");
		break;
	}
}
```

