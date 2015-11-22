// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class SHARDS_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

	UENUM() enum Type {
		LINEAR                UMETA(DisplayName = "Linear"),
		WAVE                  UMETA(DisplayName = "Wave"),
		BLINK                 UMETA(DisplayName = "Blink")
	};

private:
	float timer;
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	virtual float nderiv(float x, float ctime, float(*f)(float, float), float c = 0.01);

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) USceneComponent* StartPosition;
	UPROPERTY(EditAnywhere) USceneComponent* EndPosition;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* Model;

	/* Time in seconds it takes to complete one cycle. */
	UPROPERTY(EditAnywhere, Category = "Movement") float CycleTime;

	/** The type of movement pattern this platform uses. */
	UPROPERTY(EditAnywhere, Category = "Movement") TEnumAsByte<Type> MovementType;

	/* The platform's current velocity. */
	FVector Velocity;
	
};
