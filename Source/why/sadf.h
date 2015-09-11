// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "sadf.generated.h"

UCLASS()
class WHY_API Asadf : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Asadf();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FVector velocity;
	FVector acceleration;
};
