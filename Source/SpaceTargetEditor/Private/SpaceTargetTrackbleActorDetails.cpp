#include "SpaceTargetTrackbleActorDetails.h"
#include <Templates/SharedPointer.h>
#include <DetailCategoryBuilder.h>
#include <DetailWidgetRow.h>
#include <Input/Reply.h>
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Misc/Paths.h"
#include <AssetSelection.h>
#include "SpaceTargetDefinition.h"
#include "SSpaceTargetDownloadWidget.h"

#define LOCTEXT_NAMESPACE "SpaceTargetEditorModule"

FString emptyStr("---empty---");

TArray<TSharedPtr<FString>> SpaceTargetTrackbleActorDetails::options;

TSharedRef<IDetailCustomization> SpaceTargetTrackbleActorDetails::MakeInstance()
{
	return MakeShareable(new SpaceTargetTrackbleActorDetails);
}

void SpaceTargetTrackbleActorDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	//SpaceTargetTrackbleActor* actor;
	// These details only apply when adding an instance of the actor in a level
	if (!DetailBuilder.HasClassDefaultObject() && DetailBuilder.GetDetailsView() && DetailBuilder.GetDetailsView()->GetSelectedActorInfo().NumSelected > 0)
	{
		// Create a category so this is displayed early in the properties

		IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory(SpaceTargetDefinition::SpaceTargetTitle);

		TSharedPtr<IPropertyHandle> dataBaseHandle = DetailBuilder.GetProperty("dataBase");

		FString fristElement;
		auto result = dataBaseHandle->GetValue(fristElement);
		if (result != FPropertyAccess::Success)
		{
			fristElement = emptyStr;
		}
		FString assetsPath = SpaceTargetDefinition::dataAssetsPath();

		TArray<FString> files;

		//cause each tap actor it will add again ,so we need to clear it and add a "---empty---" element again.
		options.Empty();
		options.Add(MakeShareable(new FString(emptyStr)));

		int32 dataIndex = 0;

		IFileManager::Get().FindFiles(files, *(assetsPath + "/*"), false, true);
		if (files.Num() > 0)
		{
			IPlatformFile& ipf = FPlatformFileManager::Get().GetPlatformFile();
			int tempIndex = 0;
			for (const FString& i : files)
			{
				FString curr = assetsPath + i +"/" +SpaceTargetDefinition::sfmbin;
				if (ipf.FileExists(*curr))
				{
					tempIndex++;
					if (i.Equals(fristElement))
					{
						dataIndex = tempIndex;
					}
					//exist sfm data ,add it
					options.Add(MakeShareable(new FString(i)));
				}
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *curr);
			}
		}

		MyCategory.AddCustomRow(FText::GetEmpty())
		.NameContent()
		[
			SNew(STextBlock).Text(FText::FromString("Data"))
		]
		.ValueContent()
		[
			SAssignNew(currComboBox,SComboBox<TSharedPtr<FString>>).OptionsSource(&options)
			[
				SAssignNew(currOptionTextBlock, STextBlock).Text(FText::FromString(*(options[dataIndex])))
			]
			.OnGenerateWidget_Lambda([](TSharedPtr<FString> item)
			{
				//frist genarator widget will call a loop to this item
				return SNew(STextBlock).Text(FText::FromString(*item));
			}).OnSelectionChanged_Lambda([this, dataBaseHandle](TSharedPtr<FString> item, ESelectInfo::Type Type)
			{
				if (currOptionTextBlock)
				{
					currOptionTextBlock->SetText(FText::FromString(*item));
				}

				if (dataBaseHandle->IsValidHandle())
				{
					dataBaseHandle->SetValue(*item);
				}
			})
			.OnComboBoxOpening_Lambda([this]()
				{
					options.Empty();
					options.Add(MakeShared<FString>(FString("---empty---")));
					TArray<FString> files = FSpaceTargetEditorModule::GetAllLocalScene();
					int currIndex = 0;
					FString currentStr = currOptionTextBlock->GetText().ToString();
					for (int i = 0; i < files.Num(); i++)
					{
						options.Add(MakeShared<FString>(files[i]));
						if (currentStr.Equals(files[i]))
						{
							currIndex = i + 1;
						}
					}
					currComboBox->SetSelectedItem(options[currIndex]);
					currComboBox->RefreshOptions();
				})
		];

		auto DataOnclick = []() -> FReply
		{
			FName SSpaceTargetDownladWidget = FName("SSpaceTargetDownladWidget");
			//UE_LOG(LogSpaceTarget,Log,TEXT("click! DataOnclick"));
			FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SSpaceTargetDownladWidget,FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& SpawnTabArgs)
			{
				return SNew(SDockTab)
				[
					SNew(SSpaceTargetDownloadWidget)
				];
			}))
			.SetDisplayName(LOCTEXT("SpaceTargetEditor","DownloadAssets"))
			.SetMenuType(ETabSpawnerMenuType::Hidden);
			FGlobalTabmanager::Get()->TryInvokeTab(SSpaceTargetDownladWidget);

			return FReply::Handled();
		};

		MyCategory.AddCustomRow(FText::GetEmpty())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Download"))
		]
		.ValueContent()
		[
			SNew(SButton).Text(FText::FromString("Download Data"))
			.OnClicked_Lambda(DataOnclick)
		];
	}
}

#undef LOCTEXT_NAMESPACE