// If you wanna live to see tomorrow, you'd
// better start fryin' them eggs a little bit
// better than what you've a been fryin'. I'm 
// tired of eatin sloppy, slimey eggs.

#include "shards.h"
#include "Gem.h"

// Sets default values
AGem::AGem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(60.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Collectible"));
	SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	SetActorEnableCollision(true);

	gemKind = FMath::RandRange(1, 6);
	gemColor = FMath::RandRange(1, 6);
	GemModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(*("/Game/Models/Gems/Gem" + FString::FromInt(gemKind)));
	GemModel->SetStaticMesh(MeshObj.Object);
	GemModel->SetCollisionProfileName(TEXT("NoCollision"));
	GemModel->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	GemModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	GemModel->SetCastShadow(false);
	GemModel->AttachTo(RootComponent);

	//PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	//PointLightComponent->Intensity = 500.0f;

	//PointLightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//PointLightComponent->AttachTo(RootComponent);
	curTime = FMath::FRandRange(0.0f, 2.0f*3.14f);
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> blue(TEXT("/Game/Textures/Gems/mGem1"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> green(TEXT("/Game/Textures/Gems/mGem2"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> purple(TEXT("/Game/Textures/Gems/mGem3"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> pink(TEXT("/Game/Textures/Gems/mGem4"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> yellow(TEXT("/Game/Textures/Gems/mGem5"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> cyan(TEXT("/Game/Textures/Gems/mGem6"));
	switch (gemColor)
	{
		case 1: GemModel->SetMaterial(0, blue.Object);		break;
		case 2: GemModel->SetMaterial(0, green.Object);		break;
		case 3: GemModel->SetMaterial(0, purple.Object);	break;
		case 4: GemModel->SetMaterial(0, pink.Object);		break;
		case 5: GemModel->SetMaterial(0, yellow.Object);	break;
		case 6: GemModel->SetMaterial(0, cyan.Object);		break;
	}
}

// Called when the game starts or when spawned
void AGem::BeginPlay()
{
	Super::BeginPlay();

	//baseHeight = GetActorrelativeloca().Z;
}

// Called every frame
void AGem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector loc = GetActorLocation();
	loc = FVector(0.0f, 0.0f, 0.5f*FMath::Cos(curTime));
	SetActorLocation(GetActorLocation()+loc);
	AddActorWorldRotation(FRotator(0.0f, 60.0f*DeltaTime, 0.0f));

	curTime += DeltaTime;
}

