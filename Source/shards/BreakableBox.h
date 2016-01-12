// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BreakableBox.generated.h"

UCLASS()
class SHARDS_API ABreakableBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableBox();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere) UStaticMeshComponent* Model;
	UPROPERTY(EditAnywhere) UDestructibleComponent* boom;
};
