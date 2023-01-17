// Copyright 4DAGE Games, Inc. All Rights Reserved.
//SpaceTargetModule.h
#pragma once


#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
DECLARE_LOG_CATEGORY_EXTERN(LogSpaceTarget, Log, All);

/**
 * FSpaceTargetModule
 */
class SPACETARGET_API FSpaceTargetModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */

	/**
	 * StartupModule()
	 */
	virtual void StartupModule() override;

	/** IModuleInterface implementation */
	virtual void ShutdownModule() override;
};

