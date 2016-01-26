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

	/* The text to display. */
	UPROPERTY(EditAnywhere) FString asdf;
	
	/* Blueprint that should execute when this cut is acivated. */
	UPROPERTY(EditAnywhere) AActor* BlueprintToExecute;

	/* Length of time that this cut should last. If less than zero, the cut won't be timed one. */
	UPROPERTY(EditAnywhere) float CutDuration;

	/* Disables display of the textbox. Generally for use with CutDuration and a blueprint that runs a matinee. */
	UPROPERTY(EditAnywhere) bool NoText;
	
	float cuttimer;

};
