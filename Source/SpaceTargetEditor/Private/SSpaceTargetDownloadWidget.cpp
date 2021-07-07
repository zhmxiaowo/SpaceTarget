
#include "SSpaceTargetDownloadWidget.h"
#include <Widgets/Layout/SGridPanel.h>
#include <Widgets/SBoxPanel.h>
#include <Widgets/Text/SMultiLineEditableText.h>
#include "SpaceTargetDefinition.h"
#include "Http.h"
#include <Async/Async.h>
#include <Widgets/Notifications/SNotificationList.h>
#include <Framework/Notifications/NotificationManager.h>


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
						bool result = false;
						AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,[this,&result, infoptr,scene]()
						{
							result = DownloadAssetsBySceneId(scene);
							if (this->IsVolatile())
							{
								downloading = false;
							}
							infoptr->Fadeout();
							AsyncTask(ENamedThreads::GameThread,[result]()
							{
								FNotificationInfo Infonotice(LOCTEXT("noticesuccess", "SpaceTarget download success! please import the model and make sure 'Uniform scale' is 100"));
								Infonotice.ExpireDuration = 10.0f;
								if (!result)
								{
									Infonotice.Text = LOCTEXT("noticeSuccess", "SpaceTarget download failure! please retry");
									Infonotice.bUseSuccessFailIcons = true;
								}
								FSlateNotificationManager::Get().AddNotification(Infonotice);
							});

							if (result)
							{
								//TODO: reimport sfm_unreal.obj ,set Uniform scale = 100,set vertex color by obj line :v x y z r g b
							}
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
