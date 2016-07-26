// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "TelePad.h"
#include "EngineUtils.h" 

// Sets default values
ATelePad::ATelePad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StickModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Generic/Environment/Misc/Dais/Dais"));
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
	//PointLight->AttachTo(RootComponent);
	PointLight->SetupAttachment(RootComponent);//4.12
	PointLight->CastShadows = false;
	PointLight->SetRelativeLocation(FVector(0.0f, 0.0f, 6.8f));
	PointLight->Intensity = 0.0f; 1385.76f;
	PointLight->AttenuationRadius = 256.0f;
	PointLight->bUseInverseSquaredFalloff = false;
	PointLight->LightFalloffExponent = 0.0001f;
	PointLight->LightColor = FColor(230, 255, 235);

	Here = CreateDefaultSubobject<USceneComponent>(TEXT("Here"));
	//Here->AttachTo(StickModel);
	Here->SetupAttachment(StickModel);
	Here->SetRelativeLocation(FVector::ZeroVector);
	PostTeleportVelocity = FVector(0.0f, 0.0f, 0.0f);

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> daismat(TEXT("/Game/Generic/Environment/Misc/Dais/mDais"));
	Material = daismat.Object;
}

void ATelePad::PostInitializeComponents() {
	Super::PostInitializeComponents();
	dmat = UMaterialInstanceDynamic::Create(Material, this);
	StickModel->SetMaterial(0, dmat);
}

// Called when the game starts or when spawned
void ATelePad::BeginPlay()
{
	Super::BeginPlay();
	StickModel->SetMaterial(0, dmat);
}

// Called every frame
void ATelePad::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	gohere = Here->GetComponentLocation();
	if (dmat != nullptr) {
		dmat->SetScalarParameterValue("thing", asfd);
	}
}

void ATelePad::SetLit(bool lit) {
	asfd = lit;
}