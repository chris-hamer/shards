// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD

#include "shards.h"
#include "Auyron.h"
#include "SandShip.h"


// Sets default values
ASandShip::ASandShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsSettings.AccelRate = 5000.0f;
	PhysicsSettings.CoastingDrag = 0.75f;
	PhysicsSettings.BrakingDrag = 1.5f;
	PhysicsSettings.MaxVelocity = 10000.0f;
	PhysicsSettings.TurnRate = 7.5f;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(150.0f, 300.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetEnableGravity(false);
	CapsuleComponent->SetLinearDamping(0.0f);
	CapsuleComponent->SetAngularDamping(3.0f);
	CapsuleComponent->BodyInstance.bLockRotation = true;
	CapsuleComponent->BodyInstance.bLockXRotation = true;
	CapsuleComponent->BodyInstance.bLockYRotation = true;
	CapsuleComponent->BodyInstance.bLockZRotation = false;
	CapsuleComponent->BodyInstance.PositionSolverIterationCount = 16;
	CapsuleComponent->BodyInstance.VelocitySolverIterationCount = 16;
	CapsuleComponent->bShouldUpdatePhysicsVolume = true;
	CapsuleComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
	CapsuleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/Models/crystal"));
	Model->SetStaticMesh(MeshObj.Object);
	Model->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	Model->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Model->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->TargetArmLength = 2000.0f;
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

	MovementComponent = CreateDefaultSubobject<UAuyronMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CapsuleComponent;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface> celshade(TEXT("/Game/Textures/Effects/celshader"));
	CelShaderMaterial = celshade.Object;

	MovementInput = FVector::ZeroVector;
}

void ASandShip::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	celshadermat = UMaterialInstanceDynamic::Create(CelShaderMaterial, this);
	Camera->AddOrUpdateBlendable(celshadermat);
}

// Called when the game starts or when spawned
void ASandShip::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentDrag = PhysicsSettings.CoastingDrag;
	MovementComponent->playerradius = CapsuleComponent->GetScaledCapsuleRadius();
	MovementComponent->playerhalfheight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	MovementComponent->minnormalz = 0.4;
}

// Called every frame
void ASandShip::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (Player == nullptr) {
		CapsuleComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
		return;
	}

	Player->SetActorLocation(GetActorLocation() + 260.0f*FVector::UpVector);
	Player->PlayerModel->SetWorldRotation(FRotator(0.0f,GetActorRotation().Yaw,0.0f));

	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch + CameraInput.Y*DeltaTime*180.0f, -89.0f, 89.0f);
	NewSpringArmRotation.Yaw += CameraInput.X*DeltaTime*180.0f;
	SpringArm->SetWorldRotation(NewSpringArmRotation);

	MovementInput = MovementInput.GetClampedToSize(0.0f, 1.0f);
	FVector Forward = FVector::VectorPlaneProject(CapsuleComponent->GetForwardVector(), FVector::UpVector).GetSafeNormal();
	FVector Right = FVector::VectorPlaneProject(CapsuleComponent->GetRightVector(), FVector::UpVector).GetSafeNormal();

	//FHitResult groundtrace;
	//FCollisionShape traceshape = FCollisionShape::MakeSphere(CapsuleComponent->GetScaledCapsuleRadius()-5.0f);
	//FCollisionQueryParams gtparams;
	//gtparams.AddIgnoredActor(this);
	//GetWorld()->SweepSingleByChannel(groundtrace, GetActorLocation(), GetActorLocation() - 1000.0f*FVector::UpVector, FQuat::Identity, ECC_Visibility, traceshape, gtparams);
	//if (groundtrace.IsValidBlockingHit()) {
	//	SetActorLocation(GetActorLocation()*FVector(1.0f, 1.0f, 0.0f) + FVector::UpVector*groundtrace.ImpactPoint.Z + FVector::UpVector*CapsuleComponent->GetScaledCapsuleHalfHeight(), false, nullptr, ETeleportType::TeleportPhysics);
	//}

	FVector Velocity2D = FVector::VectorPlaneProject(CapsuleComponent->GetPhysicsLinearVelocity(),FVector::UpVector);
	if (CurrentDrag > 1.0f) {
		MovementInput.Y = 0.0f;
	}

	FVector AdjustedMovementInput = MovementInput.Y * Forward;
	CapsuleComponent->AddForce(AdjustedMovementInput*PhysicsSettings.AccelRate,NAME_None,true);
	
	float mult = FMath::Lerp(FMath::Sqrt(Velocity2D.Size() / PhysicsSettings.MaxVelocity), Velocity2D.Size() / PhysicsSettings.MaxVelocity, Velocity2D.Size() / PhysicsSettings.MaxVelocity)* CurrentDrag;
	float tangent = FMath::Pow(1.0f - FMath::Abs(Velocity2D.GetSafeNormal() | Forward),0.25f);
	CapsuleComponent->AddForce(-Velocity2D.GetSafeNormal()*PhysicsSettings.AccelRate*mult, NAME_None, true);
	if (CurrentDrag == PhysicsSettings.CoastingDrag) {
		//Root->AddForce(Right.GetSafeNormal()*PhysicsSettings.AccelRate*MovementInput.X*mult/1.0f, NAME_None, true);
	}

	if (!MovementComponent->onground) {
		CapsuleComponent->AddForce(Player->PhysicsSettings.Gravity*FVector::UpVector, NAME_None, true);
	}

	CapsuleComponent->SetPhysicsLinearVelocity(Velocity2D.Size()*Forward*FMath::Sign(Velocity2D |Forward) + CapsuleComponent->GetPhysicsLinearVelocity().Z*FVector::UpVector);

	float thing = FMath::Lerp(FMath::Clamp(DeltaTime,0.0f,1.0f), 1.0f, FMath::Lerp((Velocity2D.Size() / PhysicsSettings.MaxVelocity)/2.0f,FMath::Pow(Velocity2D.Size() / PhysicsSettings.MaxVelocity, 8.0f), (Velocity2D.Size() / PhysicsSettings.MaxVelocity)));
	AdjustedTurnInput = FMath::Lerp(AdjustedTurnInput, MovementInput.X, DeltaTime);
	CapsuleComponent->AddTorque(FVector::UpVector*AdjustedTurnInput*PhysicsSettings.TurnRate*FMath::Clamp(1.4f - FMath::Pow(Velocity2D.Size() / PhysicsSettings.MaxVelocity, 1.5f),0.0f,1.0f), NAME_None, true);
	//CapsuleComponent->BodyInstance.bLockZRotation = false;
	MovementComponent->grounddetecttfudgefactor = FMath::Clamp(1.0f - FMath::Pow(Velocity2D.Size() / PhysicsSettings.MaxVelocity,0.5f),0.0f,1.0f);
	//CapsuleComponent->SetWorldRotation(CapsuleComponent->GetComponentRotation() + FRotator(0.0,MovementInput.X*PhysicsSettings.TurnRate *(1.1f - FMath::Pow(Velocity2D.Size() / PhysicsSettings.MaxVelocity, 1.5f))*DeltaTime,0.0f), NAME_None,nullptr, ETeleportType::TeleportPhysics);
	//CapsuleComponent->BodyInstance.bLockZRotation = true;

}

// Called to bind functionality to input
void ASandShip::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveX", this, &ASandShip::MoveRight);
	InputComponent->BindAxis("MoveY", this, &ASandShip::MoveForward);
	InputComponent->BindAxis("CameraPitch", this, &ASandShip::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &ASandShip::YawCamera);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASandShip::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ASandShip::Unjump);
	InputComponent->BindAction("Use", IE_Pressed, this, &ASandShip::GetOut);
}

void ASandShip::MoveForward(float AxisValue) {
	FVector(AxisValue, 0.0f, 0.0f);
	MovementInput.Y = AxisValue;
}

void ASandShip::MoveRight(float AxisValue) {
	FVector(0.0f, AxisValue, 0.0f);
	MovementInput.X = AxisValue;//
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::SanitizeFloat(Velocity2D.Size() / PhysicsSettings.MaxVelocity));
}

void ASandShip::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void ASandShip::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void ASandShip::Jump() {
	CurrentDrag = (PhysicsSettings.BrakingDrag);
}

void ASandShip::Unjump() {
	CurrentDrag = (PhysicsSettings.CoastingDrag);
}

void ASandShip::GetOut() {

	if (Player != nullptr) {
		((APlayerController*)GetController())->Possess(Player);
		Player->CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		Player->CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		Player->ResumeInput();
		Player = nullptr;
	}
}