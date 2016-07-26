
#pragma once

#include "GameFramework/Actor.h"
#include "ShardsCharacter.h"
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
	UPROPERTY(EditAnywhere) UParticleSystemComponent* CollectionParticles;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	void GetCollected(AShardsCharacter* itsame);
	void Ded();

	AActor* Player;

	UMaterialInterface* BaseGemMaterial;
	UMaterialInstanceDynamic* gemmat;

	TArray<UStaticMesh*> meshes;

	FLinearColor GemColor;
	
	FTimerHandle PostCollectionTimer;

private:
	float curTime;
	float baseHeight;
	int32 gemKind;
};
