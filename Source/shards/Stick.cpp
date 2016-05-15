// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "Stick.h"
#include "EngineUtils.h" 

// Sets default values
AStick::AStick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StickModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Models/Environment/Dais"));
	StickModel->SetStaticMesh(MeshObj.Object);
	StickModel->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	StickModel->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
	StickModel->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	StickModel->SetCastShadow(false);
	StickModel->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	StickModel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	StickModel->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	RootComponent = StickModel;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->AttachTo(RootComponent);
	//PointLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);//4.12
	PointLight->CastShadows = false;
	PointLight->SetRelativeLocation(FVector(0.0f, 0.0f, 6.8f));
	PointLight->Intensity = 0.0f; 1385.76f;
	PointLight->AttenuationRadius = 256.0f;
	PointLight->bUseInverseSquaredFalloff = false;
	PointLight->LightFalloffExponent = 0.0001f;
	PointLight->LightColor = FColor(230, 255, 235);

	Here = CreateDefaultSubobject<USceneComponent>(TEXT("Here"));
	Here->AttachTo(StickModel);
	//Here->AttachToComponent(StickModel, FAttachmentTransformRules::KeepRelativeTransform);
	Here->SetRelativeLocation(FVector::ZeroVector);
	PostTeleportVelocity = FVector(0.0f, 0.0f, 0.0f);

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> daismat(TEXT("/Game/Textures/Environment/mDais"));
	Material = daismat.Object;
}

void AStick::PostInitializeComponents() {
	Super::PostInitializeComponents();
	dmat = UMaterialInstanceDynamic::Create(Material, this);
	StickModel->SetMaterial(0, dmat);
}

// Called when the game starts or when spawned
void AStick::BeginPlay()
{
	Super::BeginPlay();
	StickModel->SetMaterial(0, dmat);
}

// Called every frame
void AStick::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	gohere = Here->GetComponentLocation();
	if (dmat != nullptr) {
		dmat->SetScalarParameterValue("thing", asfd);
	}
}

void AStick::SetLit(bool lit) {
	asfd = lit;
}