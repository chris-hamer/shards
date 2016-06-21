// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "CameraOverrideRegion.h"
#include "TwoDimensionalMovementRegion.generated.h"

UENUM() namespace MovementRegionLockedAxis {
	enum Axis {
		XAXIS                UMETA(DisplayName = "X"),
		YAXIS                UMETA(DisplayName = "Y"),
		ZAXIS                UMETA(DisplayName = "Z")
	};
}

UCLASS()
class SHARDS_API ATwoDimensionalMovementRegion : public AActor
{
	GENERATED_BODY()
		
public:

	// Sets default values for this actor's properties
	ATwoDimensionalMovementRegion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) UBoxComponent* Region;
	UPROPERTY(EditAnywhere) TEnumAsByte<MovementRegionLockedAxis::Axis> LockedAxis;
	UPROPERTY(EditAnywhere) float LockedCoordinate;
	UPROPERTY(EditAnywhere) USplineComponent* path;
	
};
