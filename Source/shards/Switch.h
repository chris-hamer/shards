// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Switch.generated.h"

UCLASS()
class SHARDS_API ASwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

	UPROPERTY(EditAnywhere)	UStaticMeshComponent* SwitchModel;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FVector playerLoc;
	bool playerNearby;
	bool activate;
};
