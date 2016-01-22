// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "AuyronWallJumpMovementComponent.h"


UAuyronWallJumpMovementComponent::UAuyronWallJumpMovementComponent()
{}

void UAuyronWallJumpMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	FVector DesiredMovementThisFrame = ConsumeInputVector();
	FHitResult* Wall = NULL;
	wallnormal = FVector::ZeroVector;
	//MoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Wall, ETeleportType::None);
	if (Wall != nullptr) {
		wallnormal = Wall->Normal;
	}
}

