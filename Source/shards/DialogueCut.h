// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DialogueCut.generated.h"

UCLASS()
class SHARDS_API ADialogueCut : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueCut();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere) ADialogueCut* Next;

	UPROPERTY(EditAnywhere) FString asdf;
	
};