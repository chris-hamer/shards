// physics physics physics physics physics physics physics physics physics physics physics
// physics physics physics physics physics physics physics physics physics physics physics
// physics physics physics physics physics physics physics physics physics physics physics

#include "shards.h"
#include "AuyronMovementComponent.h"
#include "stick.h"
#include "DestructibleBox.h"
#include "MovingPlatform.h"
#include "RotatingPlatform.h"

UAuyronMovementComponent::UAuyronMovementComponent()
{
	offGroundTime = 0.0f;
	groundverticalvelocity = 0.0f;
	enforcementtimer = -1.0f;
	minnormalz = 0.9f;
	timerlimit = 0.15f;
}

void UAuyronMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector InputVector = ConsumeInputVector();
	FVector Horiz = FVector::VectorPlaneProject(InputVector, FVector::UpVector);
	FVector Vert = InputVector.Z*FVector::UpVector;
	FHitResult HitResult;

	SafeMoveUpdatedComponent(Horiz, UpdatedComponent->GetComponentRotation(), true, HitResult);
	if (HitResult.IsValidBlockingHit()) {
		Wall = FHitResult(HitResult);
		if (Wall.GetActor() == nullptr || !Wall.GetActor()->IsA(AMovingPlatform::StaticClass())) {
			WallNormal = Wall.Normal;
		}
		SlideAlongSurface(Horiz, 1.0 - HitResult.Time, HitResult.Normal, HitResult);
	}
	AActor* ActorToIgnore = HitResult.GetActor();

	SafeMoveUpdatedComponent(Vert, UpdatedComponent->GetComponentRotation(), true, HitResult);
	if (HitResult.IsValidBlockingHit()) {
		Floor = FHitResult(HitResult);
		SlideAlongSurface(Vert, 1.0 - HitResult.Time, HitResult.Normal, HitResult);
	}

	FHitResult ShapeTraceResult;
	FCollisionShape shape = FCollisionShape::MakeCapsule(40.0f, 40.0f); //25,25

	FCollisionQueryParams Params;
	Params.bFindInitialOverlaps = true;
	if (ActorToIgnore != nullptr) {
		Params.AddIgnoredActor(ActorToIgnore);
	}

	// Telepads don't count.
	for (TActorIterator<AStick> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		Params.AddIgnoredActor(ActorItr.operator->());
	}

	// Neither do destructibles.
	for (TActorIterator<ADestructibleBox> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// When they're broken, that is.
		if (ActorItr->fadetimer >= 0.0f) {
			Params.AddIgnoredActor(ActorItr.operator->());
		}
	}

	FCollisionResponse a;
	GetWorld()->SweepSingleByChannel(ShapeTraceResult, UpdatedComponent->GetComponentLocation()+20.0f*FVector::UpVector, UpdatedComponent->GetComponentLocation() - 1000.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, shape, Params); //100

	FVector PlayerCapsuleBottom = UpdatedComponent->GetComponentLocation() - 90.0f*FVector::UpVector; // 50
	float DistanceFromImpact = (PlayerCapsuleBottom - ShapeTraceResult.ImpactPoint).Z;
	float RequiredDistance = (onground ? 50.0f : 10.0f); //50,1
	if (!onground) {
		offGroundTime += DeltaTime;
	}

	toosteep = false;
	if (enforcementtimer >= 0.0f) {
		enforcementtimer += DeltaTime;
		toosteep = true;
	}
	
	bool wasonground = onground;
	onground = false;
	groundvelocity = FVector::ZeroVector;
	platformangularfrequency = 0.0f;
	platformspindir = 1;
	FloorNormal = FVector::ZeroVector;

	if ((enforcementtimer < timerlimit && ShapeTraceResult.Normal.Z>0.6f) && ShapeTraceResult.IsValidBlockingHit() && DistanceFromImpact < RequiredDistance && (PlayerVelocity.Z <= 0.0f || wasonground)) { // 
		if (ShapeTraceResult.Normal.Z < minnormalz) {
			if (enforcementtimer == -1.0f) {
				enforcementtimer = 0.0f;
			}
		} else {
			enforcementtimer = -1.0f;
		}
		onground = true;
		offGroundTime = 0.0f;

		// It's a moving platform.
		if (ShapeTraceResult.GetActor() != nullptr && ShapeTraceResult.GetActor()->GetClass() != nullptr && (ShapeTraceResult.GetActor()->GetClass() == AMovingPlatform::StaticClass() || ShapeTraceResult.GetActor()->GetClass() == ARotatingPlatform::StaticClass())) {
			// Record the platform's velocity and acceleration so the character controller can deal with it.
			groundvelocity = ((AMovingPlatform*)ShapeTraceResult.GetActor())->Velocity;
			groundvelocity *= (((AMovingPlatform*)ShapeTraceResult.GetActor())->Deactivated ? 0.0f : 1.0f);

			// It's a ROTATING platform.
			if (ShapeTraceResult.GetActor()->GetClass() == ARotatingPlatform::StaticClass() && ((ARotatingPlatform*)ShapeTraceResult.GetActor())->AngularPeriod > 0.0f) {
				// Add its rotational velocity, which we get my multiplying its magnitude (angular frequency
				// times distance from center) by the unit vector in the angular direction (which we get by crossing
				// the player's displacement from the center with the z axis), then make it negative if the platform
				// is rotating clockwise.  
				FVector displacement = FVector::VectorPlaneProject(GetActorLocation() - ((ARotatingPlatform*)ShapeTraceResult.GetActor())->Model->GetComponentLocation(),FVector::UpVector);
				platformangularfrequency = 2.0f * 3.14159f / ((ARotatingPlatform*)ShapeTraceResult.GetActor())->AngularPeriod;
				platformangularfrequency *= (((ARotatingPlatform*)ShapeTraceResult.GetActor())->Deactivated ? 0.0f : 1.0f);
				platformspindir = (((ARotatingPlatform*)ShapeTraceResult.GetActor())->SpinDirection == ARotatingPlatform::CW ? -1 : 1);
				groundvelocity += platformangularfrequency * displacement.Size() *
									FVector::CrossProduct(displacement,FVector::UpVector).GetSafeNormal() *
									platformspindir;
			}
		}
	}

	bool TraceBlocked;
	FVector newlocation = UpdatedComponent->GetComponentLocation();

	FHitResult TraceHitResult;
	GetWorld()->LineTraceSingleByChannel(TraceHitResult, UpdatedComponent->GetComponentLocation(), ShapeTraceResult.ImpactPoint + (ShapeTraceResult.ImpactPoint - UpdatedComponent->GetComponentLocation()).GetSafeNormal(), ECC_Visibility);
	TraceBlocked = GetWorld()->LineTraceSingleByChannel(TraceHitResult, ShapeTraceResult.ImpactPoint, ShapeTraceResult.ImpactPoint - RequiredDistance*FVector::UpVector, ECC_Visibility);
	TraceBlocked = GetWorld()->LineTraceSingleByChannel(TraceHitResult, ShapeTraceResult.ImpactPoint + 1.0f*FVector::UpVector, ShapeTraceResult.ImpactPoint - 10.0f*FVector::UpVector, ECC_Visibility);
	if (TraceHitResult.Normal.Z > minnormalz) {
		enforcementtimer = -1.0f;
	}

	if (onground) {

		if (TraceBlocked) {
			newlocation.Z = TraceHitResult.ImpactPoint.Z + 90.0f; // 50
			GetWorld()->LineTraceSingleByChannel(TraceHitResult, ShapeTraceResult.ImpactPoint + 1.0f*FVector::UpVector, ShapeTraceResult.ImpactPoint - 10.0f*FVector::UpVector, ECC_Visibility);
			FloorNormal = TraceHitResult.ImpactNormal;
		}

		SafeMoveUpdatedComponent(newlocation - UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentRotation(), true, HitResult);
		SlideAlongSurface(newlocation - UpdatedComponent->GetComponentLocation(), 1.0 - HitResult.Time, HitResult.Normal, HitResult);
	}
};