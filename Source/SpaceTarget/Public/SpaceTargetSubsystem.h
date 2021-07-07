// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SpaceTargetModule.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpaceTargetTrackbleActor.h"
#include "SpaceTargetDefinition.h"
#include "SpaceTargetSubsystem.generated.h"

//on spacetarget found delegate. for c++
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpaceTargetFound, const FOutData);
//on spacetarget lost delegate. for c++
//DECLARE_MULTICAST_DELEGATE(FOnSpaceTargetLost);

//on spacetarget found delegate. for blueprint
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpaceTargetFoundBP, const FOutData, pose);
//on spacetarget lost delegate. for blueprint
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpaceTargetLostBP);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpaceTargetPoseChangeBP, const AActor*,actor,const FTransform,pose);

UCLASS()
class SPACETARGET_API USpaceTargetSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;//{ return true; }
    virtual void Initialize(FSubsystemCollectionBase& Collection)override;
    virtual void Deinitialize()override;

    FDelegateHandle tickHandle;
protected:
    UPROPERTY(BlueprintReadWrite, Category = "4DMR-SpaceTarget")
    TScriptInterface<ISlamGenerator> stSlam;
    UPROPERTY(BlueprintReadWrite, Category = "4DMR-SpaceTarget")
    ASpaceTargetTrackbleActor* stActor;
    bool needToTick = false;

    FCameraPose _cPose;
    FCameraPose _cNewPose;
    FCameraIntrisics _cIntrisics;
    FCameraTexture _cTexture;
    int currStatus;

    bool isInit;
    bool isRelocate;

    int successCount;
    int failureCount;
    const int maxSuccessCount = 3;
    const int maxFailureCount = 20;
    const int minimunNFA = -10;
    FOutData oldData;
    FOutData useData;

    struct ImageAixs
    {
    public:
        FString X;
        FString Y;
        FString Z;
    };
    ImageAixs aixs;

public:
    UFUNCTION(BlueprintCallable, Category = "4DMR-SpaceTarget")
    bool Init(const ASpaceTargetTrackbleActor* actor, const TScriptInterface<ISlamGenerator>& platformSlam);
    UFUNCTION(BlueprintCallable, Category = "4DMR-SpaceTarget")
    bool Start();
    UFUNCTION(BlueprintCallable, Category = "4DMR-SpaceTarget")
    void Stop();

    UPROPERTY(BlueprintAssignable, Category = "4DMR-SpaceTarget")
    FOnSpaceTargetFoundBP onSpaceTargetFoundBP;
    UPROPERTY(BlueprintAssignable, Category = "4DMR-SpaceTarget")
    FOnSpaceTargetLostBP onSpacetTargetLostBP;
    UPROPERTY(BlueprintAssignable, Category = "4DMR-SpaceTarget")
    FOnSpaceTargetPoseChangeBP onSpaceTargetPoseChangeBP;

    //FOnSpaceTargetFound onSpaceTargetFound;
    //FOnSpaceTargetLost onSpaceTargetLost;
protected:
    bool Tick(float Deltatime);
    bool SimpleTick(float Deltatime);

private:
    void FixImageAixs(FImageDirection dir, ImageAixs& tempaix);
};
