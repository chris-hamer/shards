// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#include "shards.h"
#include "EquipmentPickup.h"


// Sets default values
AEquipmentPickup::AEquipmentPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	Root->InitCapsuleSize(100.0f, 100.0f);
	Root->AttachTo(RootComponent);

	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	Model->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Model->AttachTo(Root);

}

// Called when the game starts or when spawned
void AEquipmentPickup::BeginPlay()
{
	Super::BeginPlay();
	
	start = GetActorLocation();

}

// Called every frame
void AEquipmentPickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	t += DeltaTime;
	SetActorRelativeLocation(start + FMath::Sin(t*PI)*25.0f*FVector::UpVector);
	AddActorWorldRotation(FRotator(0.0f, DeltaTime*135.0f, 0.0f));
}

