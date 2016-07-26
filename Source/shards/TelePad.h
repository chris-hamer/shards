// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TelePad.generated.h"

UCLASS()
class SHARDS_API ATelePad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATelePad();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;//
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION() void SetLit(bool lit);

	UPROPERTY(EditAnywhere) USceneComponent* Here;
	UPROPERTY(EditAnywhere)	UStaticMeshComponent* StickModel;
	UPROPERTY(EditAnywhere) UMaterialInterface* Material;
	UPROPERTY(EditAnywhere) UPointLightComponent* PointLight;
	UPROPERTY(EditAnywhere) FVector PostTeleportVelocity;

	FVector gohere;
	UMaterialInstanceDynamic* dmat;

	bool asfd;
};
