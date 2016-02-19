// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE

#include "shards.h"
#include "Engine/DestructibleMesh.h"
#include "BrokenBox.h"


// Sets default values
ABrokenBox::ABrokenBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destrictuble Mesh"));
	const ConstructorHelpers::FObjectFinder<UDestructibleMesh> dm(TEXT("/Game/Models/Box_DM"));
	DestructibleMesh->SetDestructibleMesh(dm.Object);
	DestructibleMesh->LargeChunkThreshold = 100000.0f;
	DestructibleMesh->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel3);
	DestructibleMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	DestructibleMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	DestructibleMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	DestructibleMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	DestructibleMesh->SetSimulatePhysics(true);
	DestructibleMesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ABrokenBox::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.0f);
	DestructibleMesh->ApplyDamage(10.0f, GetActorLocation(), FVector::UpVector, 10000.0f);
	
}

// Called every frame
void ABrokenBox::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABrokenBox::Boom() {
}

