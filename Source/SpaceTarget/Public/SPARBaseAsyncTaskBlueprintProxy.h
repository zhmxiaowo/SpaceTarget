// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARBlueprintProxy.h"
#include "SPARBaseAsyncTaskBlueprintProxy.generated.h"

/**
 * we want to get applearkit system ,but all the entry is been protected or private,so we found this and implement it to get the applearkit ar system.
 */
UCLASS()
class SPACETARGET_API USPARBaseAsyncTaskBlueprintProxy : public UARBaseAsyncTaskBlueprintProxy
{
	GENERATED_BODY()
	
public:
	/** we need to change ARSystem from protected to public. */
	static const TWeakPtr<FARSupportInterface, ESPMode::ThreadSafe>& GetARSystemPublic();
};
