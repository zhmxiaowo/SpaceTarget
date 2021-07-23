#pragma once

#include "CoreMinimal.h"
#include "SpaceTargetDefinition.generated.h"

#define DECIMALFLOAT(fl,n)(int(fl * pow(10,n))* powf(0.1f,n))


class SPACETARGET_API SpaceTargetDefinition
{
public:
	/**
	 * when we change the data we should change the id
	 * @param id the id is scene data id
	 */
	static void Initialize(FString id);

	/**
	 * save the location data ,normal : Content/SpaceTarget/Assets
	 */
	static FString dataAssetsPath();

	/**
	 * save the showing model data ,normal : Content/SpaceTarget/Models
	 */
	static FString dataModelsPath();

	/// @brief the unreal path for model
	/// @return the relative path of the engine.
	static FString dataModelInnerPath();
	/**
	 * platform system persistent save path(sand box)
	 */
	static FString persistentPath();

	/**
	 * temp path for run/ar
	 */
	static FString persistentPathInner();

	/**
	 * AbsolutePath
	 * @param path xxxxx
	 */
	static FString AbsolutePath(FString path);

	/** SpaceTargetTitle */
	static const FName SpaceTargetTitle;

	/** location data file name(1/3)*/
	static const FString sfmbin;
	/** location data file name(2/3) */
	static const FString sfmdesc;
	/** location data file name(3/3) */
	static const FString sfmfeat;
	/** location data file name(4/4) */
	static const FString sfmobj;

	/** sfm model */
	static const FString sfmmodel;

	/** tf data file */
	static const FString tflite;
	/** tf data file encrpy. */
	static const FString enctflite;
	/**  dataurl */ 
	static const FString dataurl;

private:
	/** inner _dataID */
	static FString _dataID;
};


//inside of space target property
USTRUCT(BlueprintType)
/**
 * @struct FromSFMPose
 * @brief inside of space target property
 * @details this will return from location,we use this to locate the camera or target postion and rotation
 */
struct FromSFMPose
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FVector position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FQuat rotation;
};

UENUM(BlueprintType, Category = "4DMR-SpaceTarget")
/** 
 * determin the rotation between camera image and unreal engine coordinate system. 
 * @details normal image is up image,hardware would give it back to different rotation image
 */
enum class FImageDirection : uint8
{
	
	LEFT = 2,
	RIGHT = 3,
	UP = 0,
	DOWN = 1
};


USTRUCT(BlueprintType)
/** 
 * FAlgrithomAixs 
 */
struct FAlgrithomAixs
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FString X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FString Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FString Z;
};


USTRUCT(BlueprintType)
/** 
 * SpaceTarget plugin return orignal data.to locate and rotate target or camera.
 */
struct FOutData
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	float NFA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FromSFMPose poseOfAR;
};


USTRUCT(BlueprintType)
/** 
 * FCameraTexture 
 */
struct FCameraTexture
{
	GENERATED_USTRUCT_BODY()
public:
	/** then data length */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	int32 length;
	/** the camera direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FImageDirection direction;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	/** image data,the fomat is RGBA8888 */
	uint8* data;
};


USTRUCT(BlueprintType)
/** 
 * FCameraPose 
 */
struct FCameraPose
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	/** position */
	FVector position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	/** rotation */
	FQuat rotation;
};

USTRUCT(BlueprintType)
/** 
 * FCameraIntrisics from camera hardware and software setting.
 */
struct FCameraIntrisics
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	/** focal */
	FVector2D focal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	/** pricipal */
	FVector2D pricipal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	/** resolution */
	FIntPoint resolution;
};

UENUM(BlueprintType, Category = "4DMR-SpaceTarget")
/** 
 * FXRTrackingState is current AR system running status.
 */
enum class FXRTrackingState : uint8
{
	/** The tracking quality is good. */
	Tracking = 0,
	/** The tracking quality is not available. */
	Lost = 1,
	/** The tracking quality is limited, relying only on the device's motion. */
	TrackingUnStable = 2,
	/** The tracking quality is not available. */
	None = 3
};

UENUM(BlueprintType, Category = "4DMR-SpaceTarget")
/** 
 * FWorldCenterMode
 * @Details if you wanna show static or baking target,you should use the Target. 
 */
enum class FWorldCenterMode : uint8
{
	/** target will static,xr camera will move to target relative pose. */
	Target = 0,
	/** target will follow the xr camera relative pose. */
	Device = 1
};




