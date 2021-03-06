#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SUserWidget.h"

class SSpaceTargetDownloadWidget : public SUserWidget
{
public:

	SLATE_USER_ARGS(SSpaceTargetDownloadWidget)
	{}
	SLATE_END_ARGS()
	// MUST Provide this function for SNew to call!
	virtual void Construct(const FArguments& InArgs);
	virtual bool DownloadAssetsBySceneId(FString sceneId);
	static  bool HttpDownload(const FString& url,const FString& savePath,float timeout);
	//force multiply download.
	static bool downloading;


	static const FString donwloadUrl;
	static const FString eurUrl;

private:
	TSharedPtr<SEditableTextBox> sceneText;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> serverCompboBox;
	TSharedPtr<STextBlock> serverText;
	TArray<TSharedPtr<FString>> serverOptions;
	int serverIndex ;


	TSharedPtr<SComboBox<TSharedPtr<FString>>> deleteCompboBox;
	TArray<TSharedPtr<FString>> deleteOptions;
	TSharedPtr<STextBlock> deleteText;

};

