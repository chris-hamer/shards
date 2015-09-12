// Fill out your copyright notice in the Description page of Project Settings.

#include "why.h"
#include "Stick.h"
#include "EngineUtils.h" 

// Sets default values
AStick::AStick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StickModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	StickModel->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	RootComponent = StickModel;

	Here = CreateDefaultSubobject<USceneComponent>(TEXT("Here"));
}

// Called when the game starts or when spawned
void AStick::BeginPlay()
{
	Super::BeginPlay();
	gohere = Here->GetComponentLocation();
}

// Called every frame
void AStick::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
}

