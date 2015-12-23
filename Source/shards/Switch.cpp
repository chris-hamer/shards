// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "Auyron.h"
#include "Switch.h"
#include "ShardsMatineeActor.h"


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
}

void ASwitch::Activate()
{
	if (!State) {
		State = true;
		FOutputDeviceNull ar;
		SwitchEffect->CallFunctionByNameWithArguments(TEXT("Activate"), ar, NULL, true);
	}
}

void ASwitch::Deactivate()
{
	if (State && Toggleable) {
		State = false;
		FOutputDeviceNull ar;
		SwitchEffect->CallFunctionByNameWithArguments(TEXT("Deactivate"), ar, NULL, true);
	}
}