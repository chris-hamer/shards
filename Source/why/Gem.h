// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Gem.generated.h"

UCLASS()
class WHY_API AGem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGem();

	UPROPERTY(EditAnywhere)	UStaticMeshComponent* GemModel;
	UPROPERTY(EditAnywhere) USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere) UPointLightComponent* PointLightComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	float curTime;
	float baseHeight;
};
