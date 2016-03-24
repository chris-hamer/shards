// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#include "shards.h"
#include "Gem2.h"


// Sets default values
AGem2::AGem2()
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
	gemColor = FMath::RandRange(1, 6);//
	GemModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(*("/Game/Models/Gems/Gem" + FString::FromInt(gemKind)));
	GemModel->SetStaticMesh(MeshObj.Object);

	GemModel->SetCollisionProfileName(TEXT("NoCollision"));
	GemModel->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	GemModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	GemModel->SetCastShadow(false);
	GemModel->AttachTo(RootComponent);

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> mat(TEXT("/Game/Textures/Gems/BaseGemMaterial"));
	BaseGemMaterial = mat.Object;

	curTime = FMath::FRandRange(0.0f, 2.0f*3.14f);
}

void AGem2::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	gemmat = UMaterialInstanceDynamic::Create(BaseGemMaterial, this);
	GemModel->SetMaterial(0, gemmat);
}

// Called when the game starts or when spawned
void AGem2::BeginPlay()
{
	Super::BeginPlay();

	float meh = FMath::RandRange(0.7f, 1.0f);
	FLinearColor asdf = FLinearColor::MakeRandomColor();

	gemmat->SetVectorParameterValue("Bright Color", asdf);
	gemmat->SetVectorParameterValue("Dark Color", asdf/2.0f);

	baseHeight = GetActorLocation().Z;
}

// Called every frame
void AGem2::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector loc = GetActorLocation();
	loc = FVector(0.0f, 0.0f, 0.5f*FMath::Cos(curTime));
	SetActorLocation(GetActorLocation() + loc);
	AddActorWorldRotation(FRotator(0.0f, 60.0f*DeltaTime, 0.0f));

	curTime += DeltaTime;

}

