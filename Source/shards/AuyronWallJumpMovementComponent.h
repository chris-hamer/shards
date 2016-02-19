// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "AuyronWallJumpMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UAuyronWallJumpMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	

public:
	UAuyronWallJumpMovementComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	FVector wallnormal;
	
};
