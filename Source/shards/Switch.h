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
	virtual void Tick(float DeltaSeconds) override;

	void Activate();
	void Deactivate();

	/* The state (activated / deactivated) of the switch. */
	UPROPERTY(EditAnywhere) bool State;

	/* Whether the switch can be turned back off after being turned on. */
	UPROPERTY(EditAnywhere) bool Toggleable;

	/* Maximum distance away from the switch that the player can be while still being able to activate it. */
	UPROPERTY(EditAnywhere) float MaxDistance;

	/* Blueprint that executes when the switch is pressed. */
	UPROPERTY(EditAnywhere) AActor* SwitchEffect;

};