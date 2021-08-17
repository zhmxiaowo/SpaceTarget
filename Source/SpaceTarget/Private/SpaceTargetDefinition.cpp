#include "SpaceTargetDefinition.h"

const FName SpaceTargetDefinition::SpaceTargetTitle(TEXT("4DMR-SpaceTarget"));

const FString SpaceTargetDefinition::sfmbin("sfm_ar.bin");
const FString SpaceTargetDefinition::sfmdesc("sfm_ar.desc");
const FString SpaceTargetDefinition::sfmfeat("sfm_ar.feat");
const FString SpaceTargetDefinition::sfmobj("sfm_unreal.obj");

const FString SpaceTargetDefinition::tflite("ARHelper.tflite");
const FString SpaceTargetDefinition::enctflite("EncARHelper.tflite");

const FString SpaceTargetDefinition::dataurl("");

const FString SpaceTargetDefinition::sfmmodel("sfm_ar.obj");

void SpaceTargetDefinition::Initialize(FString id)
{
	_dataID = id;
}

FString SpaceTargetDefinition::dataAssetsPath()
{
	//FString path = FPaths::ProjectPluginsDir()+ "SpaceTarget/Content/SpaceTarget/SpaceTargetAssets/";
	FString path = FPaths::ProjectContentDir() + "SpaceTarget/SpaceTargetAssets/";
	return path;
}

FString SpaceTargetDefinition::dataModelsPath()
{
	//FString path = FPaths::ProjectPluginsDir() + "SpaceTarget/Content/SpaceTarget/Models/";
	FString path = FPaths::ProjectContentDir() + "SpaceTarget/Models/";
	return path;
}

FString SpaceTargetDefinition::dataModelInnerPath()
{
	return FString("StaticMesh'/Game/SpaceTarget/Models/{0}/sfm_unreal.sfm_unreal'");
}

FString SpaceTargetDefinition::persistentPath()
{
	FString path = FPaths::ProjectPersistentDownloadDir() + FString::Format(TEXT("/{0}/"), { _dataID });;
	return path;
}

FString SpaceTargetDefinition::persistentPathInner()
{
	FString path = FPaths::ProjectPersistentDownloadDir() + FString::Format(TEXT("/{0}/ar/"), { _dataID });;

	return path;
}

FString SpaceTargetDefinition::AbsolutePath(FString path)
{
#if PLATFORM_IOS
	IPlatformFile& ipf = FPlatformFileManager::Get().GetPlatformFile();
	path = ipf.ConvertToAbsolutePathForExternalAppForRead(*path);
#endif
	return path;
}

FString SpaceTargetDefinition::_dataID;
 