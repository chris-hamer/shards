// Fill out your copyright notice in the Description page of Project Settings.

#include "why.h"
#include "Gem.h"


// Sets default values
AGem::AGem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(20.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Collectible"));
	SetActorEnableCollision(true);

	gemKind = FMath::RandRange(1, 6);
	GemModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(*("/Game/Models/Gems/Gem" + FString::FromInt(gemKind)));
	GemModel->SetStaticMesh(MeshObj.Object);
	GemModel->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
	GemModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	GemModel->AttachTo(RootComponent);

	// This part doesn't work and I don't know why
	// It's making me depressed
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> MatObj(*("Material'/Game/Models/Gems/mGem" + FString::FromInt(gemKind) + "'"));
	GemModel->SetMaterial(0, MatObj.Object);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLightComponent->Intensity = 500.0f;
	switch (gemKind)
	{
		case 1: PointLightComponent->LightColor = FColor::Blue;		break;
		case 2: PointLightComponent->LightColor = FColor::Green;	break;
		case 3: PointLightComponent->LightColor = FColor::Magenta;	break;
		case 4: PointLightComponent->LightColor = FColor::Red;		break;
		case 5: PointLightComponent->LightColor = FColor::Yellow;	break;
		case 6: PointLightComponent->LightColor = FColor::White;	break;
	}
	PointLightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PointLightComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AGem::BeginPlay()
{
	Super::BeginPlay();

	baseHeight = GetActorLocation().Z;
	curTime = FMath::FRandRange(0.0f, 2.0f*3.14f);
}

// Called every frame
void AGem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector loc = GetActorLocation();
	loc = FVector(loc.X, loc.Y, baseHeight + 20.0f*FMath::Sin(curTime));
	SetActorLocation(loc);

	curTime += DeltaTime;
}

