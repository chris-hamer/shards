// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TwoDimensionalMovementRegion.generated.h"

UCLASS()
class SHARDS_API ATwoDimensionalMovementRegion : public AActor
{
	GENERATED_BODY()
		
public:

	UENUM() enum AxisEnum {
		XAXIS                UMETA(DisplayName = "X"),
		YAXIS                UMETA(DisplayName = "Y"),
		ZAXIS                UMETA(DisplayName = "Z")
	};

	// Sets default values for this actor's properties
	ATwoDimensionalMovementRegion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) UBoxComponent* Region;
	UPROPERTY(EditAnywhere) TEnumAsByte<AxisEnum> LockedAxis;
	UPROPERTY(EditAnywhere) float LockedCoordinate;
	
};
