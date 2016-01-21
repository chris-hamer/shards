// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "DialogueCut.h"
#include "NPC.generated.h"

UCLASS()
class SHARDS_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void Activate();

	UPROPERTY(EditAnywhere) ADialogueCut* RootCut;
	ADialogueCut* CurrentCut;
	
	/* Maximum distance away from the switch that the player can be while still being able to activate it. */
	UPROPERTY(EditAnywhere) float MaxDistance;
};
