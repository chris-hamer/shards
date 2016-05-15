// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovingPlatform.h"
#include "RotatingPlatform.generated.h"

UENUM() namespace RotatingPlatformDirection {
	enum Enum {
		CW                UMETA(DisplayName = "Clockwise"),
		CCW               UMETA(DisplayName = "Counterclockwise")
	};
}
/**
 * 
 */
UCLASS()
class SHARDS_API ARotatingPlatform : public AMovingPlatform
{
	GENERATED_BODY()
public:

	ARotatingPlatform();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere) USceneComponent* Center;

	// The time in seconds it takes the platform to complete a full rotation.
	UPROPERTY(EditAnywhere, Category = "Movement") float AngularPeriod;

	// The direction (clockwise or counterclockwise) that the platform rotates when looked at from the top down.
	UPROPERTY(EditAnywhere, Category = "Movement") TEnumAsByte<RotatingPlatformDirection::Enum> SpinDirection;

};
