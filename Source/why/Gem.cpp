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

	GemModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/StarterContent/Props/SM_Lamp_Wall"));
	GemModel->SetStaticMesh(MeshObj.Object);
	GemModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	GemModel->AttachTo(RootComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLightComponent->Intensity = 500.0f;
	PointLightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PointLightComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AGem::BeginPlay()
{
	Super::BeginPlay();

	baseHeight = GetActorLocation().Z;
	curTime = 0.0f;
}

// Called every frame
void AGem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector loc = GetActorLocation();
	loc = FVector(loc.X, loc.Y, baseHeight + 50.0f*FMath::Sin(curTime));
	SetActorLocation(loc);

	curTime += DeltaTime;
}

