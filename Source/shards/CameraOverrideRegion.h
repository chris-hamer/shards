// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CameraOverrideRegion.generated.h"

UCLASS()
class SHARDS_API ACameraOverrideRegion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraOverrideRegion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UPROPERTY(EditAnywhere) UBoxComponent* Region;
	UPROPERTY(EditAnywhere) USceneComponent* TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Camera") bool LookAtPlayer;
	UPROPERTY(EditAnywhere, Category = "Camera") FVector TargetDisplacement;
	UPROPERTY(EditAnywhere, Category = "Camera") FRotator TargetRotation;
	
};
