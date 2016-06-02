// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "Checkpoint.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	RespawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnPoint"));
	//RespawnPoint->AttachTo(Root);
	RespawnPoint->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);//4.12

	Region = CreateDefaultSubobject<UBoxComponent>(TEXT("Region"));
	//Region->AttachTo(Root);
	Region->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);//4.12
	Region->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

