// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DestructibleBox.generated.h"

UCLASS()
class SHARDS_API ADestructibleBox : public AActor
{
	GENERATED_BODY()
	
private:

public:	
	// Sets default values for this actor's properties
	ADestructibleBox();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void BeginFadeout();

	float fadetimer;
	UPROPERTY(EditAnywhere) float Lifespan;
	UPROPERTY(EditAnywhere) float FadeDelay;
	UPROPERTY(EditAnywhere) UDestructibleComponent* Mesh;
	
};
