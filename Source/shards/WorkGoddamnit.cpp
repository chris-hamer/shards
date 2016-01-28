// STOP TELLING ME TO FILL OUT MY COPYRIGHT NOTICE

#include "shards.h"
#include "WorkGodamnitMovementComponent.h"
#include "WorkGoddamnit.h"


// Sets default values
AWorkGoddamnit::AWorkGoddamnit()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	Root->SetCapsuleSize(25.0f, 50.0f);
	Root->AttachTo(RootComponent);

	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	Model->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Model->AttachTo(Root);
	Model->AddLocalOffset(-50.0f*FVector::UpVector);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->AttachTo(Root);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(SpringArm);
	MC = CreateDefaultSubobject<UWorkGodamnitMovementComponent>(TEXT("Movement Component"));
	MC->UpdatedComponent = Root;

	vroom = 200.0f;
	grav = 200.0f;
}

// Called when the game starts or when spawned
void AWorkGoddamnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorkGoddamnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Temporary variable to hold the camera's new rotation.
	FRotator NewRotation = SpringArm->GetComponentRotation();

	// Move the camera's yaw in response to the x input of the mouse/stick.
	NewRotation.Yaw = NewRotation.Yaw + CameraInput.X;

	// Set the rotation of the camera.
	SpringArm->SetWorldRotation(NewRotation);
	NewRotation = SpringArm->GetComponentRotation();
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -85.0f, 85.0f);
	SpringArm->SetWorldRotation(NewRotation);

	FVector Right = Camera->GetRightVector();
	Right.Z = 0.0f;
	Right = Right.GetSafeNormal();
	FVector Forward = Camera->GetForwardVector();
	Forward.Z = 0.0f;
	Forward = Forward.GetSafeNormal();
	FVector Acceleration = (Right*MovementInput.X + Forward*MovementInput.Y)*vroom;
	bool otg = MC->otg;
	if (!otg) {
		Acceleration.Z = -grav;
	}
	//Acceleration.Z = -100.0f;
	MC->AddInputVector(Acceleration*DeltaTime);
}

// Called to bind functionality to input
void AWorkGoddamnit::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveX", this, &AWorkGoddamnit::MoveRight);
	InputComponent->BindAxis("MoveY", this, &AWorkGoddamnit::MoveForward);
	InputComponent->BindAxis("CameraPitch", this, &AWorkGoddamnit::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AWorkGoddamnit::YawCamera);
	InputComponent->BindAxis("ControllerCameraYaw", this, &AWorkGoddamnit::ControllerYawCamera);
	//InputComponent->BindAction("Jump", IE_Pressed, this, &AWorkGoddamnit::Jump);
	//InputComponent->BindAction("Jump", IE_Released, this, &AWorkGoddamnit::Unjump);
}


// Can you believe the tutorial wanted me to use Y for horizontal movement
void AWorkGoddamnit::MoveRight(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

// and X for vertical? I mean who does that?
void AWorkGoddamnit::MoveForward(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AWorkGoddamnit::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AWorkGoddamnit::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AWorkGoddamnit::ControllerYawCamera(float AxisValue)
{
	CameraInput.X += AxisValue;
}
