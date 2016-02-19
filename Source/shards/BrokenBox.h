// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE

#pragma once

#include "GameFramework/Actor.h"
#include "BrokenBox.generated.h"

UCLASS()
class SHARDS_API ABrokenBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrokenBox();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Boom();

	UPROPERTY(EditAnywhere) UDestructibleComponent* DestructibleMesh;

	
};
