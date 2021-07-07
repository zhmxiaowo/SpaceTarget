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

	/** 
	* Deprecated model defalut name 
	* @brief test of brief
	* @exception <exception-object> {exception description} 对一个异常对象进行注释。
	* 
	* @warning {warning message } 一些需要注意的事情
	* 
	* @todo { things to be done } 对将要做的事情进行注释，链接到所有TODO 汇总的TODO 列表
	* 
	* @bug 缺陷，链接到所有缺陷汇总的缺陷列表
	* 
	* @see {comment with reference to other items } 一段包含其他部分引用的注释，中间包含对其他代码项的名称，自动产生对其的引用链接。
	* 
	* @relates <name> 通常用做把非成员函数的注释文档包含在类的说明文档中。
	* 
	* @since {text} 通常用来说明从什么版本、时间写此部分代码。
	* 
	* @deprecated
	* 
	* @pre { description of the precondition } 用来说明代码项的前提条件。
	* 
	* @post { description of the postcondition } 用来说明代码项之后的使用条件。
	* 
	* @code 在注释中开始说明一段代码，直到@endcode命令。
	* 
	* @endcode 注释中代码段的结束。
	* 
	* @code .. @endcode 包含一段代码
	* 
	* @addtogroup 添加到一个组。
	* 
	* @brief  概要信息
	* 
	* @deprecated 已废弃函数
	* 
	* @details  详细描述
	* 
	* @note  开始一个段落，用来描述一些注意事项
	* 
	* @par  开始一个段落，段落名称描述由你自己指定
	* 
	* @param  标记一个参数的意义
	* 
	* @fn  函数说明
	* 
	* @ingroup 加入到一个组
	* 
	* @return  描述返回意义
	* 
	* @retval  描述返回值意义
	* 
	* @include 包含文件
	*/
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




