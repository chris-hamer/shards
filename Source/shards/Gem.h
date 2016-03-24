
#pragma once

#include "GameFramework/Actor.h"
#include "Gem.generated.h"

UCLASS()
class SHARDS_API AGem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGem();

	UPROPERTY(EditAnywhere)	UStaticMeshComponent* GemModel;
	UPROPERTY(EditAnywhere) USphereComponent* SphereComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	FColor GemColor;
	UMaterialInterface* BaseGemMaterial;
	UMaterialInstanceDynamic* gemmat;

	TArray<UStaticMesh*> meshes;

private:
	float curTime;
	float baseHeight;
	int32 gemKind;
	int32 gemColor;
};
