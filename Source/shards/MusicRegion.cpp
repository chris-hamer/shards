// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "MusicRegion.h"


// Sets default values
AMusicRegion::AMusicRegion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Region = CreateDefaultSubobject<UBoxComponent>(TEXT("Region"));
	Region->AttachTo(Root);
	Region->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

// Called when the game starts or when spawned
void AMusicRegion::BeginPlay()
{
	Super::BeginPlay();
	if (PlayOnLevelLoad) {
		Music->Play();
	}
}

// Called every frame
void AMusicRegion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

