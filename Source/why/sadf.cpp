// Fill out your copyright notice in the Description page of Project Settings.

#include "why.h"
#include "sadf.h"


// Sets default values
Asadf::Asadf()
{
	velocity = FVector(0.0f, 0.0f, 0.0f);
	acceleration = FVector(0.0f, 0.0f, 9.0f);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Asadf::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Asadf::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FVector pos = GetActorLocation();
	velocity += acceleration*DeltaTime;
	pos += velocity*DeltaTime;
	SetActorLocation(pos);
}

