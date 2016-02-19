// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ForceRegion.generated.h"

UCLASS()
class SHARDS_API AForceRegion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForceRegion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Force Region Interface")
	void Enable();
	UFUNCTION(BlueprintCallable, Category = "Force Region Interface")
	void Disable();

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) USceneComponent* Source;
	UPROPERTY(EditAnywhere) UBoxComponent* Region;

	/* Direction of the force. */
	UPROPERTY(EditAnywhere) FVector Direction;

	/* Magnitude of the force. */
	UPROPERTY(EditAnywhere) float Magnitude;
	
};
