// physics physics physics physics physics physics physics physics physics physics physics
// physics physics physics physics physics physics physics physics physics physics physics
// physics physics physics physics physics physics physics physics physics physics physics

#include "why.h"
#include "AuyronMovementComponent.h"

UAuyronMovementComponent::UAuyronMovementComponent()
{
	offGroundTime = 0.0f;
}

//void UAuyronMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
void UAuyronMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get (and then clear) the movement vector that we set in AAuyron::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector();

	// Separate movememt vector into horizonal and vertical components.
	FVector Horiz = FVector::VectorPlaneProject(DesiredMovementThisFrame, FVector::UpVector);
	FVector Vert = FVector(0.0f, 0.0f, DesiredMovementThisFrame.Z);

	// Move vertically.
	SafeMoveUpdatedComponent(Vert, UpdatedComponent->GetComponentRotation(), true, Floor);

	// Is the slope too steep? I don't think it is. But it might be.
	bool toosteep = false;

	// Looks like we're standing on something.
	if (Floor.Normal.Z > FMath::Sin((maxslope))) {
		onground = true;
		offGroundTime = 0.0f;
	} else { // (greater than 0 so this doesn't trigger on jumps)
		// Slope is too steep so we should be pushed back by it by taking its vertical normal into account.
		Horiz += Vert;
		toosteep = true;
	}

	// If we're not on a standable slope then we are not on the ground.
	if (Floor.Normal.Z <= FMath::Sin((maxslope))) {
		offGroundTime += DeltaTime; // Give player a small window to still jump after leaving a ledge
	}

	// If we're off the ground for too long, remove the second chance to jump
	if (offGroundTime > MaxOffGroundTime) {
		onground = false;
	}

	// Sets "Horizontal" to be along the slope you're standing on so long as the slope isn't too steep.
	if (!toosteep&&!Floor.Normal.Equals(FVector::UpVector,0.1f)) {
		// Look at this math. LOOK AT THIS MATH.
		Horiz.Z = Floor.Normal.RotateAngleAxis(FMath::Sign(Horiz | Floor.Normal)*-100.0f, FVector::CrossProduct(Floor.Normal, FVector::UpVector).GetSafeNormal()).Z*Horiz.Size();
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
