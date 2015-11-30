// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "MovingPlatform.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CycleTime = 6.0f;
	MovementType = LINEAR;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//Root->AttachTo(RootComponent);

	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Models/Environment/platform"));
	Model->SetStaticMesh(MeshObj.Object);
	//Model->AttachTo(RootComponent);
	RootComponent = Model;
	Model->SetRelativeLocation(FVector::ZeroVector);

	StartPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Start Position"));
	StartPosition->AttachTo(Root);
	
	EndPosition = CreateDefaultSubobject<USceneComponent>(TEXT("End Position"));
	EndPosition->AttachTo(Root);

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	Model->SetWorldLocation(StartPosition->GetComponentLocation());
}

// Crappy makeshift numerical derivative function because I coluldn't find one in FMath.
float AMovingPlatform::nderiv(float x, float ctime, float (*f)(float,float), float c)
{
	return (((*f)(x + c, ctime) - (*f)(x, ctime)) / c);
}

// Defines wave motion where the platform's position changes sinusoidally with time.
float wave(float x, float ctime) {
	return (-FMath::Cos(2 * 3.14159 * x / ctime) + 1.0f) / (2.0f);
}

// Defines linear motion where the platform's position changes linearly with time.
float linear(float x, float ctime) {
	return (2 * FMath::Abs(FMath::Fmod(x / ctime + 0.5f, 1.0f) - 0.5f));
}

// Defines blinking behavior where the platform switches back and forth between its start and end positions.
float blink(float x, float ctime) {
	return 0.5f*(FMath::Sign(wave(x, ctime) - 0.5f) + 1);
}

// Called every frame
void AMovingPlatform::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	// Switch the function used depending on the editor setting.
	float (*f)(float, float) = &linear;
	switch (MovementType) {
	case LINEAR:
		f = &linear;
		break;
	case WAVE:
		f = &wave;
		break;
	case BLINK:
		f = &blink;
	}

	// Increment the timer and find the platform's new position.
	timer += DeltaTime;
	FVector NewPosition = FMath::Lerp(StartPosition->GetComponentLocation(), EndPosition->GetComponentLocation(), f(timer, CycleTime));
	FVector OldPosition = Model->GetComponentLocation();

	// Estimate the platform's veloicty using a numerical derivative of its movement function.
	Velocity = FMath::Lerp(FVector::ZeroVector, EndPosition->GetComponentLocation() - StartPosition->GetComponentLocation(), nderiv(timer, CycleTime, f));
	
	FVector start = StartPosition->GetComponentLocation();
	FVector end = EndPosition->GetComponentLocation();

	// Move the platform to its new location.
	Model->SetWorldLocation(NewPosition);
	StartPosition->SetWorldLocation(start);
	EndPosition->SetWorldLocation(end);
}

