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

	gemKind = FMath::RandRange(0, 5);
	gemColor = FMath::RandRange(1, 6);//
	GemModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj1(TEXT("/Game/Models/Gems/Gem1"));
	meshes.Add(MeshObj1.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj2(TEXT("/Game/Models/Gems/Gem2"));
	meshes.Add(MeshObj2.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj3(TEXT("/Game/Models/Gems/Gem3"));
	meshes.Add(MeshObj3.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj4(TEXT("/Game/Models/Gems/Gem4"));
	meshes.Add(MeshObj4.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj5(TEXT("/Game/Models/Gems/Gem5"));
	meshes.Add(MeshObj5.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj6(TEXT("/Game/Models/Gems/Gem6"));
	meshes.Add(MeshObj6.Object);
	
	GemModel->SetCollisionProfileName(TEXT("NoCollision"));
	GemModel->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	GemModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	GemModel->SetCastShadow(false);
	GemModel->SetStaticMesh(meshes[FMath::RandRange(0, 5)]);
	GemModel->AttachTo(RootComponent);

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> mat(TEXT("/Game/Textures/Gems/BaseGemMaterial"));
	BaseGemMaterial = mat.Object;

}

void AGem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	gemmat = UMaterialInstanceDynamic::Create(BaseGemMaterial, this);
	GemModel->SetMaterial(0, gemmat);
}

// Called when the game starts or when spawned
void AGem::BeginPlay()
{
	Super::BeginPlay();

	float meh = FMath::FRandRange(0.7f, 1.0f);
	FLinearColor asdf = FLinearColor::MakeRandomColor();

	gemmat->SetVectorParameterValue("Bright Color", asdf);
	gemmat->SetVectorParameterValue("Dark Color", asdf/16.0f);

	GemModel->SetStaticMesh(meshes[FMath::RandRange(0, 5)]);
	curTime = FMath::FRandRange(0.0f, 2.0f*3.14f);

	//baseHeight = GetActorrelativeloca().Z;//
}

// Called every frame
void AGem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector loc = GetActorLocation();
	loc = FVector(0.0f, 0.0f, 10.0f*DeltaTime*FMath::Cos(curTime*PI));
	SetActorLocation(GetActorLocation()+loc);
	AddActorWorldRotation(FRotator(0.0f, 60.0f*DeltaTime, 0.0f));

	curTime += DeltaTime;
}

