// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpaceTargetBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPACETARGET_API USpaceTargetBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	//static void Foo();
	UFUNCTION(BlueprintCallable, Category = "4DMR-SpaceTarget")
	static void GetAllSceneIdByDirectory(TArray<FString>& files,FString path = "");
};
