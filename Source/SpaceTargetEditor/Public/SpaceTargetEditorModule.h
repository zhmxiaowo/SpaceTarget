// Copyright 4DAGE Games, Inc. All Rights Reserved.
//SpaceTargetModule.h
#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "UnrealEd.h"

class SPACETARGETEDITOR_API FSpaceTargetEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
