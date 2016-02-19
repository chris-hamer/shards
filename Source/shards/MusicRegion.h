// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MusicRegion.generated.h"

UCLASS()
class SHARDS_API AMusicRegion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicRegion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) UBoxComponent* Region;
	UPROPERTY(EditAnywhere) AAmbientSound* Music;
	UPROPERTY(EditAnywhere) bool PlayOnLevelLoad;
	
	
};
