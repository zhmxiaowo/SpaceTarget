// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceTargetModule.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceTargetDefinition.h"
#include "SpaceTargetTrackbleActor.generated.h"

UCLASS()
/**
 * ASpaceTargetTrackbleActor is the center of spacetarget.we init it in editor,and it will transfer data to subsystem and run logic.
 */
class SPACETARGET_API ASpaceTargetTrackbleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * default init
	 */
	ASpaceTargetTrackbleActor();

protected:
	/**
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif
	/**
	 * SetMesh use for editor change...
	 */
	void SetMesh();
	UPROPERTY(Category = StaticMeshActor, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	/**
	 * StaticMeshComponent get the mesh data
	 */
	class UStaticMeshComponent* StaticMeshComponent;

public:	
	/**
	 * Called every frame
	 * @param DeltaTime every frame delta time.
	 */
	virtual void Tick(float DeltaTime) override;

	/** Use it to determine the model and tracking data.Please use it on Details panel. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FString dataBase;

	/** decide the target move or xr camera move */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	FWorldCenterMode centerMode;

	/** Use it to show/hide the model*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4DMR-SpaceTarget")
	bool visibleModel;
};