// Fill out your copyright notice in the Description page of Project Settings.


#include "SPARBaseAsyncTaskBlueprintProxy.h"

const TWeakPtr<FARSupportInterface, ESPMode::ThreadSafe>& USPARBaseAsyncTaskBlueprintProxy::GetARSystemPublic()
{
	return GetARSystem();
}
