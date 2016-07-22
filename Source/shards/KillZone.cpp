// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#include "shards.h"
#include "KillZone.h"


// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Region = CreateDefaultSubobject<UBoxComponent>(TEXT("Region"));
	//Region->AttachTo(Root);
	Region->SetupAttachment(Root);//4.12
	Region->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKillZone::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

