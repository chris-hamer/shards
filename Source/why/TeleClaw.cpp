// Fill out your copyright notice in the Description page of Project Settings.

#include "why.h"
#include "TeleClaw.h"


// Sets default values
ATeleClaw::ATeleClaw()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeleClaw = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Models/Weapons/TeleClaw"));
	TeleClaw->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void ATeleClaw::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleClaw::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

