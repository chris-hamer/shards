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
	UAuyronMovementComponent();

	// Variables to store the floor and wall collision normals.
	FHitResult Floor;
	FHitResult Wall;

	// Temporary variable to tell the actual character class if we're on the ground or not.
	bool onground;
	float offGroundTime;
	float MaxOffGroundTime;
	FVector PlatformVelocity;

	// Temporary variable to tell the actual character class if we WERE on the ground or not.
	bool wasonground;

	// Stores the maximum slope as set by the character class.
	float maxslope;

	// Velocity of the platform you're standing on.
	FVector groundvelocity;

	// Vertical velocity of the platform you're standing on.
	// Stored separatly because the z component of groundvelocity
	// is set to zero if it is positive to compensate for collision
	// detection, but its value is still useful.
	float groundverticalvelocity;

	// Angular frequency of the rotating platform that the player is standing on.
	float platformangularfrequency;

	// Is 1 if the platform is spinning clockwise, and -1 if it isn't.
	int8 platformspindir;

	FVector wallnormal;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};