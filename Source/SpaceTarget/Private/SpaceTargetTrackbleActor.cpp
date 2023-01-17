// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceTargetTrackbleActor.h"
#include "Components/StaticMeshComponent.h"
#include "SpaceTargetDefinition.h"
#include "Engine/StaticMesh.h"

FString emptyStr = FString("---empty---");

// Sets default values
ASpaceTargetTrackbleActor::ASpaceTargetTrackbleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	auto newTransform = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(newTransform);


    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComponent->Mobility = EComponentMobility::Movable;
    StaticMeshComponent->SetupAttachment(GetRootComponent());
    StaticMeshComponent->bUseDefaultCollision = true;

	dataBase = emptyStr;
	visibleModel = true;

    SetMesh();
}

// Called when the game starts or when spawned
void ASpaceTargetTrackbleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceTargetTrackbleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASpaceTargetTrackbleActor::SetMesh()
{
    if (!StaticMeshComponent)
    {
        return;
    }
    if (dataBase == FString("---empty---"))
    {
        StaticMeshComponent->SetStaticMesh(nullptr);
        return;
    }
    FString modelPath = FPaths::Combine(SpaceTargetDefinition::dataModelsPath(),dataBase,SpaceTargetDefinition::sfmobj) ;
    if (IFileManager::Get().FileExists(*modelPath))
    {
        //FString modelUAssets = FString::Format(*FString("StaticMesh'/SpaceTarget/SpaceTarget/Models/{0}/sfm_unreal.sfm_unreal'"),{dataBase});
        FString modelUAssets = FString::Format(*SpaceTargetDefinition::dataModelInnerPath(), { dataBase });
        //set the model to this scene
        //static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*modelUAssets);
        UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *modelUAssets));
        if (mesh)
        {
            StaticMeshComponent->SetStaticMesh(mesh);
            StaticMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
            StaticMeshComponent->SetRelativeRotation(FRotator(0,0,0));
        }
    }
    else
    {
        StaticMeshComponent->SetStaticMesh(nullptr);
        UE_LOG(LogSpaceTarget, Error, TEXT("%s model not found! Path:"), *dataBase, *modelPath);
    }
}

#if WITH_EDITOR
void ASpaceTargetTrackbleActor::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
    Super::PostEditChangeProperty(e);

    FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
    if (PropertyName == GET_MEMBER_NAME_CHECKED(ASpaceTargetTrackbleActor, dataBase))
    {
        /* if you want to use bool property */
        //FStrProperty* prop = static_cast<FStrProperty*>(e.Property);
        //FString s;
        //prop->GetPropertyValue(*s);
        SetMesh();

    }
    else if (PropertyName == GET_MEMBER_NAME_CHECKED(ASpaceTargetTrackbleActor, visibleModel))
    {
        if (StaticMeshComponent)
        {
            StaticMeshComponent->SetVisibility(visibleModel, true);
        }
    }

}


#endif
