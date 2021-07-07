#pragma once

#include "SpaceTargetDefinition.h"
#include "CoreMinimal.h"
#include "SlamGenerator.generated.h"


UINTERFACE(Blueprintable)
class SPACETARGET_API USlamGenerator : public UInterface
{
	GENERATED_UINTERFACE_BODY()

};
/**
 *  Interface for the AR System. overwrite it to accept different AR location system input.
 */
class ISlamGenerator 
{
	GENERATED_IINTERFACE_BODY()
public:

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "4DMR-SpaceTarget/interface")
	//ISlamGenerator* Create();

	/** 
	 * get slam current camera intrinsics interface,you need to overwrite it.,run in thread pool
	 * @param ci struct contain focal,resolution and pricipel
	 */
	virtual bool GetCameraIntrinsics(FCameraIntrisics& ci) = 0;
	/**
	 * get slam current pose interface,you need to overwrite it.,run in thread pool
	 * @param cp struct contain postion and rotation.
	 */
	virtual bool GetCameraPose(FCameraPose& cp) = 0;
	/**
	 * if worldCenterMode is 'Target',this method will be call to set the camera position,run in game thread
	 * @param cp camera rotation and position.
	 */
	virtual void SetCameraPose(FTransform cp) = 0;
	/**
	 * get slam tracking state.you need to overwrite it.run in thread pool
	 * @param ts current AR sytem tracking status.
	 */
	virtual bool GetTrackingStatus(FXRTrackingState& ts) = 0;
	/**
	 * get slam tracking texture which type is RGBA8888,you need to overwrite it,run in thread pool
	 * @param ct struct,contain rgba bytes,bytes length and camera direction*,the camera direction determine the corresponding relationship between the image and the Unreal Engine coordinate system 
	 */
	virtual bool GetCameraTexture(FCameraTexture& ct) = 0;

};





