// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "CameraOverrideRegion.h"


// Sets default values
ACameraOverrideRegion::ACameraOverrideRegion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Region = CreateDefaultSubobject<UBoxComponent>(TEXT("Region"));
	//Region->AttachTo(Root);
	Region->SetupAttachment(Root);//4.12
	Region->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Target Camera"));
	//TargetCamera->AttachTo(Root);
	TargetCamera->SetupAttachment(Root);//4.12

	Axis = CreateDefaultSubobject<UArrowComponent>(TEXT("Axis"));
	//Axis->AttachTo(Root);
	Axis->SetupAttachment(Root);//4.12

	path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	path->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ACameraOverrideRegion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACameraOverrideRegion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

