// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "butt2.h"


// Sets default values
Abutt2::Abutt2()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Acceleration = FVector(0.0f, 0.0f, 3.0f);
}

// Called when the game starts or when spawned
void Abutt2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Abutt2::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	FVector pos = GetActorLocation();
	Velocity += Acceleration*DeltaTime;
	pos += Velocity*DeltaTime;
	SetActorLocation(pos);

}

// Called to bind functionality to input
void Abutt2::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

