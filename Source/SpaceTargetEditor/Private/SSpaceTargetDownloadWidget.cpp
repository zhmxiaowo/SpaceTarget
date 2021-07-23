
#include "SSpaceTargetDownloadWidget.h"
#include <Widgets/Layout/SGridPanel.h>
#include <Widgets/SBoxPanel.h>
#include <Widgets/Text/SMultiLineEditableText.h>
#include "SpaceTargetDefinition.h"
#include "Http.h"
#include <Async/Async.h>
#include <Widgets/Notifications/SNotificationList.h>
#include <Framework/Notifications/NotificationManager.h>
#if !PLATFORM_MAC
#include <FbxImporter.h>
#endif
#include <UObject/ConstructorHelpers.h>
#include <Factories/FbxFactory.h>
#include <AssetImportTask.h>
#include <FileHelpers.h>
#include <Misc/FileHelper.h>
#include <RawMesh/Public/RawMesh.h>
#include "ProceduralMeshComponent.h"
#include <MeshDescription.h>
#include "ProceduralMeshConversion.h"
#include "Engine/StaticMesh.h"
#include <AssetRegistry/AssetRegistryModule.h>
#include <Engine/StaticMeshActor.h>


#define LOCTEXT_NAMESPACE "SpaceTargetEditorModule"

bool SSpaceTargetDownloadWidget::downloading = false;

const FString SSpaceTargetDownloadWidget::donwloadUrl("https://4dkk.4dage.com/images/images{0}/ar/{1}");

void SSpaceTargetDownloadWidget::Construct(const FArguments& InArgs)
{
	SUserWidget::Construct(SUserWidget::FArguments()
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.FillHeight(0.382f)
		[
			SNew(SMultiLineEditableText)
			.Text(LOCTEXT("tips", "Enter the scene code of the scene, click the button to download the scene function.\n\nthe scene code can be obtained at the scene web address(behind 'm=')\n\nexmple: https://www.4dkankan.com/spc.html?m=Html34yLt \n\nthe scene id is: Html34yLt"))
		]
		+ SVerticalBox::Slot()
		.Padding(0.0f, 5.0f)
		.HAlign(HAlign_Left)
		[
			SNew(SGridPanel)
			+ SGridPanel::Slot(0, 0)
			.HAlign(HAlign_Left)
			.Padding(0.0f, 5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("SceneLabel", "Scene ID:"))
			]
			+SGridPanel::Slot(1 ,0)
			.HAlign(HAlign_Left)
			.Padding(0.0f, 5.0f)
			[
				SAssignNew(sceneText,SEditableTextBox)
				.HintText(LOCTEXT("SceneText", "enter the scene id."))
			]
			+SGridPanel::Slot(0,1)
			.Padding(0.5f, 0.5f)
			[
				SNew(SButton)
				.Text(FText::FromString("Download"))
				.OnClicked_Lambda([&]() -> FReply
				{
					if (downloading)
					{
						//error
						FMessageDialog::Open(EAppMsgType::Ok,LOCTEXT("Downloading","last mission is still downloading ,please wait..."));
					}
					else
					{
						FString scene = sceneText->GetText().ToString();
						UE_LOG(LogTemp,Log,TEXT("start download %s"),*scene);
						downloading = true;
						FNotificationInfo Info(LOCTEXT("noticeDownload", "SpaceTarget data downloading ,please wait..."));
						Info.bFireAndForget = false;
						TSharedPtr<SNotificationItem> infoptr = FSlateNotificationManager::Get().AddNotification(Info);
						
						AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,[this,&result, infoptr,scene]()
						{
							volatile bool result = false;
							result = DownloadAssetsBySceneId(scene);

							if (result)
							{
								FString modelUAssets = FString::Format(*SpaceTargetDefinition::dataModelInnerPath(), { scene });
								//sfm_unreal.obj file path
								FString objfile = FPaths::Combine(SpaceTargetDefinition::dataModelsPath(), scene, SpaceTargetDefinition::sfmobj);
								FString objstr;
								FFileHelper::LoadFileToString(objstr, *objfile);
								TArray<FString> objlines;
								objstr.ParseIntoArrayLines(objlines);

								TArray<FColor> vertexColor;
								TArray<FVector> vertex;
								TArray<int32> wedge;

								for (FString i : objlines)
								{
									TArray<FString> eachline;
									i.ParseIntoArray(eachline, TEXT(" "), true);
									if (eachline.Num() > 4 && eachline[0].ToLower() == FString("v")) // v x y z r g b
									{
										uint8 R = (uint8)FCString::Atoi(*eachline[4]);
										uint8 G = (uint8)FCString::Atoi(*eachline[5]);
										uint8 B = (uint8)FCString::Atoi(*eachline[6]);
										FColor tempColor(R, G, B);
										vertexColor.Add(tempColor);

										//coordinate x aixs reverse. and size is m to cm(unreal)
										float x = FCString::Atof(*eachline[1]) * 100;
										float y = -FCString::Atof(*eachline[2]) * 100;
										float z = FCString::Atof(*eachline[3]) * 100;
										FVector tempVector(x, y, z);
										vertex.Add(tempVector);
									}
									else
										if (eachline.Num() > 3 && eachline[0].ToLower() == FString("f")) // f 1 2 3
										{
											TArray<FString> fnum1, fnum2, fnum3;
											eachline[1].ParseIntoArray(fnum1, TEXT("/"), true);
											eachline[2].ParseIntoArray(fnum2, TEXT("/"), true);
											eachline[3].ParseIntoArray(fnum3, TEXT("/"), true);

											int32 wedge1 = FCString::Atoi(*fnum1[0]) - 1;
											int32 wedge2 = FCString::Atoi(*fnum2[0]) - 1;
											int32 wedge3 = FCString::Atoi(*fnum3[0]) - 1;
											wedge.Add(wedge1);
											wedge.Add(wedge2);
											wedge.Add(wedge3);
										}
								}
								auto task = Async(EAsyncExecution::TaskGraphMainThread, [&]()
								{
#if !PLATFORM_MAC
									if (!GWorld)//&& vertexColor.Num() < 1)
									{
										//old data use fbx import
										FString filePath = FPaths::Combine(*SpaceTargetDefinition::dataModelsPath(), *scene, *SpaceTargetDefinition::sfmobj);

										UFbxFactory* Factory = NewObject<UFbxFactory>(UFbxFactory::StaticClass(), FName("Factory"));
										Factory->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
										Factory->ImportUI->OriginalImportType = FBXIT_StaticMesh;
										Factory->ImportUI->StaticMeshImportData->ImportUniformScale = 100.0f;
										Factory->ImportUI->StaticMeshImportData->VertexColorImportOption = EVertexColorImportOption::Replace;
										Factory->ImportUI->StaticMeshImportData->bTransformVertexToAbsolute = true;
										Factory->ImportUI->bIsObjImport = true;
										UAssetImportTask* ImportTask = NewObject<UAssetImportTask>(UAssetImportTask::StaticClass());
										ImportTask->bAutomated = true;
										ImportTask->bSave = true;
										
										Factory->SetAssetImportTask(ImportTask);

										bool canceled = false;
										FString pkgName = FPaths::GetBaseFilename(SpaceTargetDefinition::sfmobj);
										FString relativePath = FString::Format(*FString("/Game/SpaceTarget/Models/{0}/{1}"), { scene , pkgName });
										UPackage* Package = CreatePackage(*relativePath);
										UObject* import = Factory->ImportObject(Factory->ResolveSupportedClass(), Package, *pkgName, RF_Public | RF_Standalone, filePath, nullptr, canceled);
										//Package->SavePackage(Package,nullptr, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *pkgName);
										Package->SetDirtyFlag(true);
										UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);
										TArray<UPackage*> packages = { Package->GetOutermost() };
										UEditorLoadingAndSavingUtils::SavePackages(packages, false);

										//FString modelUAssets = FString::Format(*SpaceTargetDefinition::dataModelInnerPath(), { scene });
										//UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *modelUAssets));
									}
									else
#endif
									{
										// save to uasset
										AStaticMeshActor* actor = Cast<AStaticMeshActor>(GWorld->SpawnActor(AStaticMeshActor::StaticClass()));
										UProceduralMeshComponent* ProcMeshComp = NewObject<UProceduralMeshComponent>(actor, "ProceduralMesh", RF_NoFlags, nullptr, false, nullptr);

										TArray<FVector> normals;
										TArray<FVector2D> uv0;
										TArray<FProcMeshTangent> tangent;
										ProcMeshComp->CreateMeshSection(0, vertex, wedge, normals, uv0, vertexColor, tangent, true);
										UMaterialInterface* matInstance = (UMaterialInterface*)StaticLoadObject(UMaterialInterface::StaticClass(), NULL, *FString("Material'/SpaceTarget/SpaceTarget/Materials/ModelMaterial.ModelMaterial'"));
										ProcMeshComp->SetMaterial(0, matInstance);

										FMeshDescription MeshDescription = BuildMeshDescription(ProcMeshComp);

										if (MeshDescription.Polygons().Num() > 0)
										{
											FString meshstr = FPaths::GetBaseFilename(SpaceTargetDefinition::sfmobj);
											FString UserPackageName = FString::Format(*FString("/Game/SpaceTarget/Models/{0}/{1}"), { scene , meshstr });
											FName MeshName(*meshstr);
											UPackage* procPackage = CreatePackage(*UserPackageName);
											// Create StaticMesh object
											UStaticMesh* StaticMesh = NewObject<UStaticMesh>(procPackage, MeshName, RF_Public | RF_Standalone);
											StaticMesh->InitResources();

											StaticMesh->LightingGuid = FGuid::NewGuid();
											FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
											SrcModel.BuildSettings.bRecomputeNormals = false;
											SrcModel.BuildSettings.bRecomputeTangents = false;
											SrcModel.BuildSettings.bRemoveDegenerates = false;
											SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
											SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
											SrcModel.BuildSettings.bGenerateLightmapUVs = true;
											SrcModel.BuildSettings.SrcLightmapIndex = 0;
											SrcModel.BuildSettings.DstLightmapIndex = 1;
											StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
											StaticMesh->CommitMeshDescription(0);
											if (!ProcMeshComp->bUseComplexAsSimpleCollision)
											{
												StaticMesh->CreateBodySetup();
												UBodySetup* NewBodySetup = StaticMesh->BodySetup;
												NewBodySetup->BodySetupGuid = FGuid::NewGuid();
												NewBodySetup->AggGeom.ConvexElems = ProcMeshComp->ProcMeshBodySetup->AggGeom.ConvexElems;
												NewBodySetup->bGenerateMirroredCollision = false;
												NewBodySetup->bDoubleSidedGeometry = true;
												NewBodySetup->CollisionTraceFlag = CTF_UseDefault;
												NewBodySetup->CreatePhysicsMeshes();
											}

											TSet<UMaterialInterface*> UniqueMaterials;
											const int32 NumSections = ProcMeshComp->GetNumSections();
											for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
											{
												FProcMeshSection* ProcSection =
													ProcMeshComp->GetProcMeshSection(SectionIdx);
												UMaterialInterface* Material = ProcMeshComp->GetMaterial(SectionIdx);
												UniqueMaterials.Add(Material);
											}
											for (auto* Material : UniqueMaterials)
											{
												StaticMesh->StaticMaterials.Add(FStaticMaterial(Material));
											}

											StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;
											// Build mesh from source
											StaticMesh->Build(false);
											StaticMesh->PostEditChange();

											// Notify asset registry of new asset
											FAssetRegistryModule::AssetCreated(StaticMesh);
											procPackage->SetDirtyFlag(true);
											UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);
										}
										actor->Destroy();
									}
								});
								//it will break current thread.
								task.Wait();
							}

							if (this->IsVolatile())
							{
								downloading = false;
							}

							infoptr->Fadeout();
							AsyncTask(ENamedThreads::GameThread,[result]()
							{
								FNotificationInfo Infonotice(LOCTEXT("noticesuccess", "SpaceTarget download success!"));
								Infonotice.ExpireDuration = 5.0f;
								if (!result)
								{
									Infonotice.Text = LOCTEXT("noticefailure", "SpaceTarget download failure! please retry.");
									Infonotice.bUseSuccessFailIcons = true;
								}
								FSlateNotificationManager::Get().AddNotification(Infonotice);
							});

							//procedual mesh to static mesh and save as uasset.
							downloading = false;
						});

					}
					return FReply::Handled();
				})
			]
		]
	]);

}

bool SSpaceTargetDownloadWidget::DownloadAssetsBySceneId(FString sceneId)
{
	//http url
	FString url_bin = FString::Format(*donwloadUrl,{sceneId,SpaceTargetDefinition::sfmbin});
	FString url_feat = FString::Format(*donwloadUrl, { sceneId,SpaceTargetDefinition::sfmfeat });
	FString url_desc = FString::Format(*donwloadUrl, { sceneId,SpaceTargetDefinition::sfmdesc });
	FString url_obj = FString::Format(*donwloadUrl, { sceneId,SpaceTargetDefinition::sfmobj });
	
	//local path
	FString path = SpaceTargetDefinition::dataAssetsPath();
	FString savepath_bin = FPaths::Combine(*path,*sceneId, *SpaceTargetDefinition::sfmbin);
	FString savepath_feat = FPaths::Combine(*path, *sceneId, *SpaceTargetDefinition::sfmfeat);
	FString savepath_desc = FPaths::Combine(*path, *sceneId, *SpaceTargetDefinition::sfmdesc);
	FString savepath_obj = FPaths::Combine(*SpaceTargetDefinition::dataModelsPath(), *sceneId, *SpaceTargetDefinition::sfmobj);

	bool bin = HttpDownload(url_bin, savepath_bin,30);
	bool feat = HttpDownload(url_feat, savepath_feat, 30);
	bool desc = HttpDownload(url_desc, savepath_desc, 30);
	bool obj = HttpDownload(url_obj, savepath_obj, 30);

	return bin && feat && desc && obj;
}


bool SSpaceTargetDownloadWidget::HttpDownload(const FString& url, const FString& savePath, float timeout)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(url);
	HttpRequest->SetTimeout(timeout);
	volatile bool downloadlock = true;
	bool result = false;

	HttpRequest->OnProcessRequestComplete().BindLambda([&downloadlock ,&result,savePath](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()) && bWasSuccessful)
		{
			// Save file
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

			// Create save directory if not existent
			FString Path, Filename, Extension;
			FPaths::Split(savePath, Path, Filename, Extension);
			if (!PlatformFile.DirectoryExists(*Path))
			{
				if (!PlatformFile.CreateDirectoryTree(*Path))
				{
					downloadlock = false;
					result = false;
					return;
				}
			}
			// Open / Create the file
			IFileHandle* FileHandle = PlatformFile.OpenWrite(*savePath);
			if (FileHandle)
			{
				// Write the file
				FileHandle->Write(Response->GetContent().GetData(), Response->GetContentLength());
				// Close the file
				delete FileHandle;


				result = true;
				downloadlock = false;
			}
			else
			{
				result = false;
				downloadlock = false;
				return;
			}
		}
		else
		{
			result = false;
			downloadlock = false;
			return;
		}
	});
	HttpRequest->ProcessRequest();

	while (downloadlock)
	{
		FPlatformProcess::Sleep(0);
	}

	return result;
}

TSharedRef<SSpaceTargetDownloadWidget> SSpaceTargetDownloadWidget::New()
{
	return MakeShareable(new SSpaceTargetDownloadWidget());
}

#undef LOCTEXT_NAMESPACE
