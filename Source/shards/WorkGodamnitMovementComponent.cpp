// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE

#include "shards.h"
#include "WorkGodamnitMovementComponent.h"

void UWorkGodamnitMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	FVector delta = ConsumeInputVector();
	FVector d = FVector::VectorPlaneProject(delta, FVector::UpVector);
	FVector dz = delta.Z*FVector::UpVector;
	FHitResult hit;
	FHitResult hit2;
	SafeMoveUpdatedComponent(d, UpdatedComponent->GetComponentRotation(), true, hit);

	if (hit.IsValidBlockingHit()) {
		SlideAlongSurface(d, 1.0 - hit.Time, hit.Normal, hit);
	}
	AActor* asdf = hit.GetActor();
	SafeMoveUpdatedComponent(dz, UpdatedComponent->GetComponentRotation(), true, hit);
	
	if (hit.IsValidBlockingHit()) {
		SlideAlongSurface(dz, 1.0 - hit.Time, hit.Normal, hit);
	}

	FHitResult f;
	FCollisionShape shape = FCollisionShape::MakeCapsule(25.0f, 25.0f);
	FCollisionShape shape2 = FCollisionShape::MakeBox(FVector(50.0f*0.707f, 50.0f*0.707f, 1.0f));
	//FCollisionShape shape = FCollisionShape::MakeCapsule(25.0f, 50.0f);

	FCollisionQueryParams bestcqparams;
	bestcqparams.bFindInitialOverlaps = true;
	if (asdf != nullptr) {
		bestcqparams.AddIgnoredActor(asdf);
	}
	GetWorld()->SweepSingleByChannel(f, UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() - 100.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, shape,bestcqparams);

	FVector tempasdf = UpdatedComponent->GetComponentLocation() - 50.0f*FVector::UpVector;
	float dist = (tempasdf - f.ImpactPoint).Z;
	float distreq = (otg ? 50.0f : 1.0f);

	otg = false;
	if (f.IsValidBlockingHit() && dist < distreq) {
		otg = true;
	}
	if (otg) {

		bool yadonedidit;
		FVector newlocation = UpdatedComponent->GetComponentLocation();

		FHitResult aaa;
		GetWorld()->LineTraceSingleByChannel(aaa, UpdatedComponent->GetComponentLocation(), f.ImpactPoint + (f.ImpactPoint - UpdatedComponent->GetComponentLocation()).GetSafeNormal(), ECC_Visibility);
		yadonedidit = GetWorld()->LineTraceSingleByChannel(aaa, f.ImpactPoint, f.ImpactPoint - distreq*FVector::UpVector, ECC_Visibility);
		if (yadonedidit) {
			newlocation.Z = aaa.ImpactPoint.Z + 50.0f;
		}
		//UpdatedComponent->SetWorldLocation(newlocation);


		yadonedidit = GetWorld()->LineTraceSingleByChannel(aaa, UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() - (51.0f)*FVector::UpVector, ECC_Visibility);
		if (yadonedidit) {
			//UpdatedComponent->SetWorldLocation(aaa.ImpactPoint + 50.0f*FVector::UpVector);
			//newlocation = (aaa.ImpactPoint + 50.0f*FVector::UpVector);
			otg = true;
		}

		if (!yadonedidit) {

		}

		if (!yadonedidit) {
			FVector what = f.ImpactPoint;
			what.Z = UpdatedComponent->GetComponentLocation().Z + 50.0f;
			//yadonedidit = GetWorld()->LineTraceSingleByChannel(hit, what, f.ImpactPoint - 1.0*FVector::UpVector, ECC_Visibility);
			what = UpdatedComponent->GetComponentLocation();
			what.Z = hit.ImpactPoint.Z;
			if (yadonedidit) {
				//UpdatedComponent->SetWorldLocation(what);
				//newlocation = what;
			}
		}

		SafeMoveUpdatedComponent(newlocation- UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentRotation(), true, hit);
		SlideAlongSurface(newlocation- UpdatedComponent->GetComponentLocation(), 1.0 - hit.Time, hit.Normal, hit);
		//UpdatedComponent->SetWorldLocation(newlocation);
	}


}




