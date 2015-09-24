// physics physics physics physics physics physics physics physics physics physics physics
// physics physics physics physics physics physics physics physics physics physics physics
// physics physics physics physics physics physics physics physics physics physics physics

#include "why.h"
#include "AuyronMovementComponent.h"
#include "MovingPlatform.h"

UAuyronMovementComponent::UAuyronMovementComponent()
{
	offGroundTime = 0.0f;
	groundverticalvelocity = 0.0f;
}

//void UAuyronMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
void UAuyronMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get (and also clear) the movement vector that we set in AAuyron::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector();

	// Separate movememt vector into horizonal and vertical components.
	FVector Horiz = FVector::VectorPlaneProject(DesiredMovementThisFrame, FVector::UpVector);
	FVector Vert = FVector(0.0f, 0.0f, DesiredMovementThisFrame.Z);

	// Move vertically.
	SafeMoveUpdatedComponent(Vert, UpdatedComponent->GetComponentRotation(), true, Floor);

	// Is the slope too steep? I don't think it is. But it might be.
	bool toosteep = false;

	// Assume you're not on a moving platform.
	groundvelocity = FVector::ZeroVector;
	groundverticalvelocity = 0.0f;

	// Looks like we're standing on something.
	if (Floor.Normal.Z > FMath::Sin((maxslope))) {
		onground = true;
		offGroundTime = 0.0f;

		// It's a moving platform.
		if (Floor.GetActor()->GetClass()->GetName() == "MovingPlatform") {
			// Record the platform's velocity so the character controller can deal with it.
			groundvelocity = ((AMovingPlatform*)Floor.GetActor())->Velocity;

			// If the platform is moving up, ignore the vertical part of its movement
			// since collision detection will push the character upwards anyway.
			if (groundvelocity.Z > 0.0f) {
				groundverticalvelocity = groundvelocity.Z;
				groundvelocity.Z = 0.0f;
			}
		}
	} else {
		// Slope is too steep so we should be pushed back by it by taking its vertical normal into account.
		Horiz += Vert;
		toosteep = true;
	}

	// If we're not on a standable slope then we are not on the ground.
	if (Floor.Normal.Z <= FMath::Sin((maxslope))) {
		onground = false;
		offGroundTime += DeltaTime;
	}

	// Sets "Horizontal" movement to be up/down the slope you're standing on so long as the slope isn't too steep.
	if (!toosteep&&!Floor.Normal.Equals(FVector::UpVector,0.1f)) {
		// Look at this math. LOOK AT THIS MATH.
		//Horiz.Z += Floor.Normal.RotateAngleAxis(FMath::Sign(Horiz | Floor.Normal)*-91.0f, FVector::CrossProduct(Floor.Normal, FVector::UpVector).GetSafeNormal()).Z*Horiz.Size();
	}

	// Move horizontally.
	SafeMoveUpdatedComponent(Horiz, UpdatedComponent->GetComponentRotation(), true, Wall);
	
	// Stop rubbing your face against the wall.
	if (Wall.IsValidBlockingHit())
	{
		// I have to do all these calculations because you're too stupid to not run into walls.
		SlideAlongSurface(Horiz, 1.f - Wall.Time, Wall.Normal, Wall, true);
	}
};
