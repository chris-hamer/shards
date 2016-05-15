// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UENUM() namespace MovingPlatformType {
	enum Enum {
		WAVE                  UMETA(DisplayName = "Wave"),
		LINEAR                UMETA(DisplayName = "Linear"),
		BLINK                 UMETA(DisplayName = "Blink"),
		ONEWAY				  UMETA(DisplayName = "OneWay")
	};
}

UCLASS()
class SHARDS_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

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

	UFUNCTION(BlueprintCallable, Category = "Moving Platform Interface") void Deactivate();
	UFUNCTION(BlueprintCallable, Category = "Moving Platform Interface") void Activate();

	UPROPERTY(EditAnywhere) USceneComponent* StartPosition;
	UPROPERTY(EditAnywhere) USceneComponent* EndPosition;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* Model;

	/* Whether or not this platform's movement sould be controlled exclusively by external mechnisms (such as blueprints).
	Using one of the MovingPlatform class's default movement patterns yields greater accuracy. */
	UPROPERTY(EditAnywhere, Category = "Movement") bool OverrideMovement;

	/* Stops the platform from moving if true. Can be used for platforms that can be activated/deactivated. */
	UPROPERTY(EditAnywhere, Category = "Movement") bool Deactivated;

	/* Time in seconds it takes to complete one cycle. */
	UPROPERTY(EditAnywhere, Category = "Movement") float CycleTime;

	/** The type of movement pattern this platform uses. */
	UPROPERTY(EditAnywhere, Category = "Movement") TEnumAsByte<MovingPlatformType::Enum> MovementType;

	/* The platform's current velocity. */
	FVector Velocity;
	
	bool HasVelocityFunction;
	FVector PreviousLocation;
	FVector PreviousVelocity;

};
