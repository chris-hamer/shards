// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "WarpCrystal.h"


// Sets default values
AWarpCrystal::AWarpCrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	WarpRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("Region"));
	WarpRegion->AttachTo(Root);
	WarpRegion->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	WarpExit = CreateDefaultSubobject<USceneComponent>(TEXT("Exit"));
	WarpExit->AttachTo(Root);

}

// Called when the game starts or when spawned
void AWarpCrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarpCrystal::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

