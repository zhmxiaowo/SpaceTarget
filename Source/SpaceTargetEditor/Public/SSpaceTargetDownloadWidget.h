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
	UFUNCTION(BlueprintCallable)
	static  bool HttpRequest(const FString& url, FString& result, FString Method = "GET", float timeout = 3);
	static  float HttpContentLength(const FString& url, float timeout = 3);
	//force multiply download.
	static bool downloading;


	static const FString donwloadUrl;
	static const FString eurUrl;

	static const FString localDomain;
	static const FString globalDomain;

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

