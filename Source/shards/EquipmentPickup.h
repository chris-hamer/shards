// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#pragma once

#include "GameFramework/Actor.h"
#include "EquipmentPickup.generated.h"

UCLASS()
class SHARDS_API AEquipmentPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipmentPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere) UCapsuleComponent* Root;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* Model;
	UPROPERTY(EditAnywhere) FString Name;

private:
	FVector start;
	float t;
};
