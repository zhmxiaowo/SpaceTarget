#pragma once

#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"

class SpaceTargetTrackbleActorDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	static TArray<TSharedPtr<FString>> options;

private:
	//show select dropdown first element.
	TSharedPtr<STextBlock> currOptionTextBlock;
};
