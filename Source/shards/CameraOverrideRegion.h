// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CameraOverrideRegion.generated.h"

UCLASS()
class SHARDS_API ACameraOverrideRegion : public AActor
{
	GENERATED_BODY()
	
public:	

	UENUM() enum ELockType {
		POINT			UMETA(DisplayName = "Point"),
		AXIS			UMETA(DisplayName = "Axis"),
		PLANE			UMETA(DisplayName = "Plane")
	};

	// Sets default values for this actor's properties
	ACameraOverrideRegion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) UBoxComponent* Region;
	UPROPERTY(EditAnywhere) UCameraComponent* TargetCamera;
	UPROPERTY(EditAnywhere) UArrowComponent* Axis;

	UPROPERTY(EditAnywhere, Category = "Camera") TEnumAsByte<ELockType> LockType;
	UPROPERTY(EditAnywhere, Category = "Camera") bool LookAtPlayer;

};
