// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MusicChangeTrigger.generated.h"

UCLASS()
class SHARDS_API AMusicChangeTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicChangeTrigger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) UBoxComponent* Region;
	UPROPERTY(EditAnywhere) AAmbientSound* Music1;
	UPROPERTY(EditAnywhere) AAmbientSound* Music2;
	
};
