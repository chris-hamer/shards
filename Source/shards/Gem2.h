// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#pragma once

#include "GameFramework/Actor.h"
#include "Gem2.generated.h"

UCLASS()
class SHARDS_API AGem2 : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGem2();

	UPROPERTY(EditAnywhere)	UStaticMeshComponent* GemModel;
	UPROPERTY(EditAnywhere) USphereComponent* SphereComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void PostInitializeComponents() override;

	FColor GemColor;
	UMaterialInterface* BaseGemMaterial;
	UMaterialInstanceDynamic* gemmat;

private:
	float curTime;
	float baseHeight;
	int32 gemKind;
	int32 gemColor;
	
};
