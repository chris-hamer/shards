// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "DestructibleBox.h"


// Sets default values
ADestructibleBox::ADestructibleBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Lifespan = 5.0f;
	FadeDelay = 3.0f;

	fadetimer = -1.0f;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetCollisionObjectType(ECC_Destructible);
	StaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->AttachTo(RootComponent);
	
}

// Called when the game starts or when spawned
void ADestructibleBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructibleBox::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (fadetimer >= 0.0f) {
		fadetimer += DeltaTime;
		BrokenBox->Boom();
	}
}

void ADestructibleBox::BeginFadeout()
{
	BrokenBox = GetWorld()->SpawnActor<ABrokenBox>(GetActorLocation(), GetActorRotation());
	Destroy();
}

