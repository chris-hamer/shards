// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "MusicChangeTrigger.h"


// Sets default values
AMusicChangeTrigger::AMusicChangeTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Region = CreateDefaultSubobject<UBoxComponent>(TEXT("Region"));
	Region->AttachTo(Root);
}

// Called when the game starts or when spawned
void AMusicChangeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMusicChangeTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
}

