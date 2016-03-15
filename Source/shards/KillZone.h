// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#pragma once

#include "GameFramework/Actor.h"
#include "KillZone.generated.h"

UCLASS()
class SHARDS_API AKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillZone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere) USceneComponent* Root;
	UPROPERTY(EditAnywhere) UBoxComponent* Region;

	
};
