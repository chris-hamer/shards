// Fill out your copyright notice in the Description page of Project Settings.

#include "why.h"
#include "Auyron.h"
#include "Switch.h"


// Sets default values
ASwitch::ASwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwitchModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SwitchModel->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	SwitchModel->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASwitch::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	for (TActorIterator<AAuyron> findPlayer(GetWorld()); findPlayer; ++findPlayer)
	{
		playerLoc = findPlayer->GetActorLocation();
		activate = findPlayer->ShouldActivate;

		// TODO: Probably some raycasting shit to make sure people can't press buttons through walls
		if (playerNearby && activate)
		{
			// This is the part where stuff is supposed to happen

			UE_LOG(LogTemp, Warning, TEXT("Pressed thing"));
		}

		findPlayer->ShouldActivate = false;
		playerNearby = FVector::DistSquared(playerLoc, GetActorLocation()) < 11000.0f;

		break;
	}
}

