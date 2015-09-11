// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "AuyronMovementComponent.generated.h"

/**
*
*/
UCLASS()
class UAuyronMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	// Variables to store the floor and wall collision normals.
	FHitResult Floor;
	FHitResult Wall;

	// Temporary variable to tell the actual character class if we're on the ground or not.
	bool onground;

	// Temporary variable to tell the actual character class if we WERE on the ground or not.
	bool wasonground;

	// Stores the maximum slope as set by the character class.
	float maxslope;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};