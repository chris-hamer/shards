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

	Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Mesh"));
	Mesh->LargeChunkThreshold = 100000.0f;
	Mesh->SetCollisionObjectType(ECC_Destructible);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	Mesh->SetSimulatePhysics(true);
	Mesh->AttachTo(RootComponent);
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
	}
}

void ADestructibleBox::BeginFadeout()
{
	if (fadetimer == -1.0f) {
		SetLifeSpan(Lifespan);
		Mesh->SetCollisionObjectType(ECC_GameTraceChannel3);
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		fadetimer = 0.0f;
	}
}

