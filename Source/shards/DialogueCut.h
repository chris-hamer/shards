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
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue Cut Interface")
	void SetText(FString text);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue Cut Interface")
	void SetNext(ADialogueCut* nextcut);

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere) ADialogueCut* Next;

	/* The text to display. */
	UPROPERTY(EditAnywhere) FString DialogueText;
	
	/* Blueprint that should execute when this cut is acivated. */
	UPROPERTY(EditAnywhere) AActor* BlueprintToExecute;

	/* Length of time that this cut should last. If less than or equal to zero, the cut won't be timed one and will act normally. */
	UPROPERTY(EditAnywhere) float CutDuration;

	/* Disables display of the textbox. Generally for use with CutDuration and a blueprint that runs a matinee. */
	UPROPERTY(EditAnywhere) bool NoText;

	/* Whether or not the camera should immediately snap to its new position. */
	UPROPERTY(EditAnywhere) bool InstantTransition;
	
	float cuttimer;

};
