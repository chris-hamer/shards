// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Stick.generated.h"

UCLASS()
class WHY_API AStick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStick();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere) USceneComponent* Here;
	UPROPERTY(EditAnywhere)	UStaticMeshComponent* StickModel;
	UPROPERTY(EditAnywhere) UPointLightComponent* PointLight;
	UPROPERTY(EditAnywhere) FVector PostTeleportVelocity;

	FVector gohere;
	
};
