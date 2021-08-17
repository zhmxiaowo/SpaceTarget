// Copyright 4DAGE Games, Inc. All Rights Reserved.

//#include "CoreMinimal.h"
#include "SpaceTargetEditorModule.h"
#include "SpaceTargetTrackbleActorDetails.h"
//#include "c:/Program Files/Epic Games/UE_4.26/Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"
#include "Modules/ModuleManager.h"
#include <Modules/ModuleManager.h>

IMPLEMENT_MODULE(FSpaceTargetEditorModule, SpaceTargetEditor);

#define LOCTEXT_NAMESPACE "SpaceTargetEditorModule"


void FSpaceTargetEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomClassLayout(FName("SpaceTargetTrackbleActor"), FOnGetDetailCustomizationInstance::CreateStatic(&SpaceTargetTrackbleActorDetails::MakeInstance));

	bool exists = false;
	FString spaceTargetStageNonUFSPath("SpaceTarget/SpaceTargetAssets");
	UProjectPackagingSettings* PackagingSettings = Cast<UProjectPackagingSettings>(UProjectPackagingSettings::StaticClass()->GetDefaultObject());
	for (FDirectoryPath dir : PackagingSettings->DirectoriesToAlwaysStageAsNonUFS)
	{
		if (dir.Path.Equals(spaceTargetStageNonUFSPath))
		{
			exists = true;
			break;
		}
	}
	if (!exists)
	{
		FDirectoryPath newDir;
		newDir.Path = spaceTargetStageNonUFSPath;
		PackagingSettings->DirectoriesToAlwaysStageAsNonUFS.Add(newDir);
		FString savePath = FPaths::ProjectConfigDir() +FString("DefaultGame.ini");
		PackagingSettings->SaveConfig(CPF_Config,*savePath);
		//TODO: this save will override to Save/Config/Game.ini not sure work it.
		//PackagingSettings->SaveConfig();
	}
}

void FSpaceTargetEditorModule::ShutdownModule()
{
	FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor");
	if (PropertyModule)
	{
		PropertyModule->UnregisterCustomClassLayout(FName("SpaceTargetTrackbleActor"));
	}
}

TArray<FString> FSpaceTargetEditorModule::GetAllLocalScene()
{
	FString assetsPath = SpaceTargetDefinition::dataModelsPath();

	int32 dataIndex = 0;
	TArray<FString> tempFiles;
	TArray<FString> Files;
	IFileManager::Get().FindFiles(tempFiles, *(assetsPath + "/*"), false, true);
	if (tempFiles.Num() > 0)
	{
		IPlatformFile& ipf = FPlatformFileManager::Get().GetPlatformFile();
		int tempIndex = 0;
		for (const FString& i : tempFiles)
		{
			FString curr = assetsPath + i + "/" + SpaceTargetDefinition::sfmobj;
			if (ipf.FileExists(*curr))
			{
				tempIndex++;
				//exist sfm data ,add it
				Files.Add(FString(i));
			}
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *curr);
		}
	}
	return Files;
}

#undef LOCTEXT_NAMESPACE
