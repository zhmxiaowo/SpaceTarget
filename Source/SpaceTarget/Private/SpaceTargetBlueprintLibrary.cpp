// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceTargetBlueprintLibrary.h"
#include "HAL/FileManager.h"
#include "SpaceTargetDefinition.h"




void USpaceTargetBlueprintLibrary::GetAllSceneIdByDirectory(TArray<FString>& files, FString path /*= ""*/)
{
	FString assetsPath = SpaceTargetDefinition::dataModelsPath();
	if (!path.IsEmpty())
	{
		assetsPath = path;
	}

	int32 dataIndex = 0;
	TArray<FString> tempFiles;
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
				files.Add(FString(i));
			}
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *curr);
		}
	}
}
