#pragma once
#include "SlamGenerator.h"
#include "SpaceTargetDefinition.h"
#include "ARKitSlamGenerator.generated.h"

/**
 *  Runtime instance. Because we want to use the GetWorld() property of AActor, we inherit AActor,otherwise you can inherit another UObject type
 */
UCLASS(BlueprintType, Blueprintable, Category = "4DMR-SpaceTarget")
class AARKitSlamGenerator : public AActor, public ISlamGenerator
{
	GENERATED_BODY()
public:

	AARKitSlamGenerator();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	/** 
	 * get slam current camera intrinsics interface,you need to overwrite it.,run in thread pool
	 * @param ci struct contain focal,resolution and pricipel
	 */
	 	virtual bool GetCameraIntrinsics(FCameraIntrisics& ci) override;

	/**
	 * get slam current pose interface,you need to overwrite it.,run in thread pool
	 * @param cp struct contain postion and rotation.
	 */
	 	virtual bool GetCameraPose(FCameraPose& cp) override;
	/**
	 * if worldCenterMode is 'Target',this method will be call to set the camera position,run in game thread
	 * @param cp camera rotation and position.
	 */
	 	virtual void SetCameraPose(FTransform cp) override;
	/**
	 * get slam tracking state.you need to overwrite it.run in thread pool
	 * @param ts current AR sytem tracking status.
	 */
	virtual bool GetTrackingStatus(FXRTrackingState& ts) override;
	/**
	 * get slam tracking texture which type is RGBA8888,you need to overwrite it,run in thread pool
	 * @param ct struct,contain rgba bytes,bytes length and camera direction*,the camera direction determine the corresponding relationship between the image and the Unreal Engine coordinate system 
	 */
	virtual bool GetCameraTexture(FCameraTexture& ct) override;

protected:
	/** inner save intrisics */
	FCameraIntrisics _intrisics;
	/** intrisics has been init*/
	bool isInitIntrinsics;
	/** inner for exchange pixels. */
	TArray<FColor> RawPixels;
	/** temp method*/
	void SaveFile(FString& path, const uint8* data, int32 len);
};

