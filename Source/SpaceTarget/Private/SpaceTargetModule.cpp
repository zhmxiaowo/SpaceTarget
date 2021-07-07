// Copyright 4DAGE Games, Inc. All Rights Reserved.

//#include "CoreMinimal.h"
#include "SpaceTargetModule.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY(LogSpaceTarget);

void FSpaceTargetModule::StartupModule()
{
	UE_LOG(LogSpaceTarget, Warning, TEXT("SpaceTarget module has start up."));
}

void FSpaceTargetModule::ShutdownModule()
{
	UE_LOG(LogSpaceTarget, Warning, TEXT("SpaceTarget module has shut down."));
}

IMPLEMENT_MODULE(FSpaceTargetModule, SpaceTarget);
