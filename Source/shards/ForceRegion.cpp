// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "ForceRegion.h"


// Sets default values
AForceRegion::AForceRegion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Source = CreateDefaultSubobject<USceneComponent>(TEXT("Source"));

	Region = CreateDefaultSubobject<UBoxComponent>(TEXT("Region"));
	Region->AttachTo(Root);
	Region->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

// Called when the game starts or when spawned
void AForceRegion::BeginPlay()
{
	Super::BeginPlay();
	Direction = Direction.GetSafeNormal();
}

// Called every frame
void AForceRegion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AForceRegion::Enable()
{
	Region->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AForceRegion::Disable()
{
	Region->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

