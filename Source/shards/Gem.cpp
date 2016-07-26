// If you wanna live to see tomorrow, you'd
// better start fryin' them eggs a little bit
// better than what you've a been fryin'. I'm 
// tired of eatin sloppy, slimey eggs.

#include "shards.h"
#include "Auyron.h"
#include "Gem.h"

// Sets default values
AGem::AGem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(120.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Collectible"));
	SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	SetActorEnableCollision(true);

	GemModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	GemColor = FLinearColor::MakeRandomColor();

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj1(TEXT("/Game/Generic/Gems/Gem1"));
	meshes.Add(MeshObj1.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj2(TEXT("/Game/Generic/Gems/Gem2"));
	meshes.Add(MeshObj2.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj3(TEXT("/Game/Generic/Gems/Gem3"));
	meshes.Add(MeshObj3.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj4(TEXT("/Game/Generic/Gems/Gem4"));
	meshes.Add(MeshObj4.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj5(TEXT("/Game/Generic/Gems/Gem5"));
	meshes.Add(MeshObj5.Object);
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj6(TEXT("/Game/Generic/Gems/Gem6"));
	meshes.Add(MeshObj6.Object);
	
	GemModel->SetCollisionProfileName(TEXT("NoCollision"));
	GemModel->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	GemModel->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	GemModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	GemModel->SetCastShadow(false);
	GemModel->SetStaticMesh(meshes[FMath::RandRange(0, 5)]);
	//GemModel->AttachTo(RootComponent);
	GemModel->SetupAttachment(RootComponent);//4.12

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> mat(TEXT("/Game/Generic/Gems/BaseGemMaterial"));
	BaseGemMaterial = mat.Object;

	CollectionParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CollectionParticles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> particles(TEXT("/Game/Effects/Player/CollectionParticles.CollectionParticles"));
	CollectionParticles->SetTemplate(particles.Object);
	//CollectionParticles->AttachTo(GemModel);
	CollectionParticles->SetupAttachment(GemModel);//4.12
	CollectionParticles->SetRelativeLocation(FVector::ZeroVector);
	CollectionParticles->bAutoActivate = false;
}

void AGem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	gemmat = UMaterialInstanceDynamic::Create(BaseGemMaterial, this);
	CollectionParticles->SetRelativeLocation(FVector::ZeroVector);
	GemModel->SetMaterial(0, gemmat);
}

// Called when the game starts or when spawned
void AGem::BeginPlay()
{
	Super::BeginPlay();

	float meh = FMath::FRandRange(0.7f, 1.0f);
	GemColor = FLinearColor::MakeRandomColor();
	CollectionParticles->SetRelativeLocation(FVector::ZeroVector);

	gemmat->SetVectorParameterValue("Bright Color", GemColor);
	gemmat->SetVectorParameterValue("Dark Color", GemColor / 32.0f);

	CollectionParticles->SetColorParameter("ParticleColor", GemColor);

	GemModel->SetStaticMesh(meshes[FMath::RandRange(0, 5)]);
	curTime = FMath::FRandRange(0.0f, 2.0f*3.14f);

	CollectionParticles->bAutoActivate = false;//
	CollectionParticles->DeactivateSystem();
	CollectionParticles->UpdateInstances();
	//baseHeight = GetActorrelativeloca().Z;//
}

// Called every frame
void AGem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	CollectionParticles->SetColorParameter("ParticleColor", GemColor);
	CollectionParticles->SetRelativeLocation(FVector::ZeroVector);
	CollectionParticles->SetWorldRotation(FRotator::ZeroRotator);
	CollectionParticles->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	//CollectionParticles->UpdateInstances();
	FVector loc = GetActorLocation();
	loc = FVector(0.0f, 0.0f, 10.0f*DeltaTime*FMath::Cos(curTime*PI));

	if (Player != nullptr) {
		CollectionParticles->SetVectorParameter("PlayerPosition", 1.0f*(Player->GetActorLocation()-GetActorLocation()));
		CollectionParticles->SetVectorParameter("PlayerSize1", Player->GetActorLocation() + (DeltaTime / 0.016f)*FVector(-45.0f, -45.0f, -45.0f));
		CollectionParticles->SetVectorParameter("PlayerSize2", Player->GetActorLocation() + (DeltaTime / 0.016f)*FVector(45.0f, 45.0f, 45.0f));
		CollectionParticles->SetFloatParameter("Strength", (1.0f/DeltaTime)*FMath::Clamp(GetWorldTimerManager().GetTimerElapsed(PostCollectionTimer) / 2.0f, 0.0f, 1.0f)*FMath::Clamp(GetWorldTimerManager().GetTimerElapsed(PostCollectionTimer) / 2.0f, 0.0f, 1.0f));
	} else {
		SetActorLocation(GetActorLocation() + loc);
		GemModel->AddWorldRotation(FRotator(0.0f, 60.0f*DeltaTime, 0.0f));
	}

	// Eliot made me put this here.
	if (GetWorldTimerManager().GetTimerElapsed(PostCollectionTimer) != -1.0f) {
		gemmat->SetScalarParameterValue("Break", 20.0f*GetWorldTimerManager().GetTimerElapsed(PostCollectionTimer));
	}

	curTime += DeltaTime;
}

void AGem::GetCollected(AAuyron* itsame) {
	Player = itsame;
	CollectionParticles->ActivateSystem();
	GemModel->SetVisibility(false);
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(PostCollectionTimer, this, &AGem::Ded, 5.0f);
}

void AGem::Ded() {
	this->Destroy();
}