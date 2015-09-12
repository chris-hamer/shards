// DON'T TELL ME WHERE TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD.

#include "why.h"
#include "Auyron.h"
#include "AuyronMovementComponent.h"

// Sets default values
AAuyron::AAuyron()
{	
	// These should work.
	AccelerationRate = 5500.0f;
	GroundDeceleration = 600.0f;
	AirDeceleration = 50.0f;
	MaxVelocity = 420.0f; // Blaze it
	MaxSlope = 45.0f;
	TurnRate = 480.0f;
	JumpPower = 500.0f;
	Gravity = 1000.0f;
	UnjumpRate = 1.5f;
	FacingAngleSnapThreshold = 5.0f;
	CameraMaxAngle = 85.0f;
	CameraMinAngle = -85.0f;
	DefaultArmLength = 400.0f;
	CameraLag = 3.0f;
	CameraAutoTurnFactor = 1.0f;
	CameraResetTime = 1.0f;
	OffGroundJumpTime = 0.08f;

	// Just in case.
	TargetDirection = FRotator::ZeroRotator;
	Velocity = FVector::ZeroVector;

	// Oh you better believe we're ticking every frame.
	PrimaryActorTick.bCanEverTick = true;

	// I wanted to be a cylinder, but no, we gotta be a capsule.
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->InitCapsuleSize(40.0f, 60.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));

	// It you.
	PlayerModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	PlayerModel->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	PlayerModel->AttachTo(RootComponent);

	// Use a spring arm so the camera can be all like swoosh.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArm->bEnableCameraLag = true;

	// Camera so the plebs can "see what they're doing" or whatever.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	// ASSUMING DIRECT CONTROL.
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Apparently we need some newfangled "MovementComponent".
	MovementComponent = CreateDefaultSubobject<UAuyronMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
}

// GAME START.
void AAuyron::BeginPlay()
{
	Super::BeginPlay();
	SpringArm->TargetArmLength = DefaultArmLength;
	SpringArm->CameraLagSpeed = CameraLag;
	MovementComponent->maxslope = MaxSlope;
	MovementComponent->MaxOffGroundTime = OffGroundJumpTime;
	Gravity = -Gravity;
}

// Called every frame UNLIKE UNITY MIRITE?
void AAuyron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update timing values.
	TimeSinceLastMouseInput += DeltaTime;

	// Move the camera in response to mouse movement.
	{
		// Reset camera input timer if the camera controls were touched.
		if (CameraInput.X != 0.0f || CameraInput.Y != 0.0f) {
			TimeSinceLastMouseInput = 0.0f;
		}

		FRotator NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Yaw = NewRotation.Yaw + CameraInput.X;
		// The camera should only turn with the player if the mouse hasn't been touched recently.
		if (TimeSinceLastMouseInput > CameraResetTime) {
			NewRotation.Yaw = NewRotation.Yaw + MovementInput.X*CameraAutoTurnFactor;
		}
		SpringArm->SetWorldRotation(NewRotation);

		NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -CameraMaxAngle, -CameraMinAngle);
		SpringArm->SetWorldRotation(NewRotation);
	}

	// Move the player in response to movement inputs.
	{
		// The directions of "Right" and "Forward" depend on the direction the camera's facing.
		FVector Right = Camera->GetRightVector();
		Right.Z = 0.0f;
		Right = Right.GetSafeNormal();
		FVector Forward = Camera->GetForwardVector();
		Forward.Z = 0.0f;
		Forward = Forward.GetSafeNormal();

		// Set up acceleration vector using the movement inputs.
		FVector2D normalInput = MovementInput.GetSafeNormal();
		FVector Acceleration = (Right*normalInput.X + Forward*normalInput.Y)*AccelerationRate;

		// Apply deceleration.
		if (OnTheGround) {
			Acceleration -= (FVector::VectorPlaneProject(Velocity, FVector(0.0f, 0.0f, 1.0f)))*GroundDeceleration*DeltaTime;
		} else {
			Acceleration -= (FVector::VectorPlaneProject(Velocity, FVector(0.0f, 0.0f, 1.0f)))*AirDeceleration*DeltaTime;
		}

		// Ask the movement component if we're on the ground
		// and apply gravity if we are.
		OnTheGround = MovementComponent->onground;
		if (!OnTheGround) {
			Acceleration += FVector(0.0f,0.0f,Gravity);
		} else if(WasOnTheGround) {
			// Push the player into the ground a bit so we still get collisions every frame.
			Velocity.Z = -10.0f;
		}

		// You done bonked yer head on that there ceiling.
		if (MovementComponent->Floor.Normal.Z < -0.6f) {
			Velocity.Z = 0.0f;
		}

		// Physiiiiicccss.
		Velocity += Acceleration * DeltaTime;

		// Only cap the horizontal movement velocity.
		float tempz;
		tempz = Velocity.Z;
		Velocity.Z = 0.0f;
		Velocity = Velocity.GetClampedToMaxSize(MaxVelocity * MovementInput.Size());
		Velocity.Z = tempz;

		// Handle jumping.
		if (JumpNextFrame) {
			// Jump taking the floor's angle into account.
			Velocity.Z = 0;
			Velocity += JumpPower*MovementComponent->Floor.Normal;
			JumpNextFrame = false;
			WasOnTheGround = false;
		}

		// Variable jump height
		if (!HoldingJump && Velocity.Z > 0) {
			Velocity += Gravity * FVector(0, 0, UnjumpRate) * DeltaTime;
		}

		// Store current on the ground state into WasOnTheGround.
		WasOnTheGround = !WasOnTheGround && MovementComponent->offGroundTime < OffGroundJumpTime ? false : OnTheGround;

		// And now we get to actually move.
		MovementComponent->AddInputVector(Velocity * DeltaTime);

		// If we're trying to move, take the camera's orientation into account to figure
		// out the direction we want to face.
		if (!MovementInput.IsNearlyZero()) {
			// I'ma tell ya not even Unity was stupid enough to use -180 -> 180 for rotations.
			int8 reflect = (MovementInput.X >= 0 ? 1 : -1);
			FVector asdf = PlayerModel->GetComponentRotation().Vector();

			// (but I forgive you)
			TargetDirection.Yaw = SpringArm->GetComponentRotation().Yaw +
				reflect * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector2D(0, 1)));
		} else {
			// If we're not moving set our target direction to the direction we're currently facing.
			TargetDirection = PlayerModel->GetComponentRotation();
		}

		// Turn to face the target direction, but do it all smooth like.
		{
			// Horrible quaternion voodoo. Viewer discretion is advised.
			// I'm honestly still not quite sure what I did.
			FQuat test = FQuat::FindBetween(PlayerModel->GetComponentRotation().Vector(), TargetDirection.Vector());
			float angle = 0.0f;
			FVector dummy;
			test.ToAxisAndAngle(dummy, angle);

			// Snap to the target angle if we're close enough, otherwise just keep turning.
			if (FMath::Abs(angle) > FMath::DegreesToRadians(FacingAngleSnapThreshold)) {
				test = FQuat(dummy, FMath::DegreesToRadians(TurnRate)*DeltaTime);
				PlayerModel->AddRelativeRotation(test);
			} else {
				PlayerModel->SetRelativeRotation(TargetDirection);
			}
		}
		// Like what even ARE quaternions anyway?
	}
}

// LOOK AT ME I'M PASSING FUNCTIONS AS ARGUMENTS PLEBS LOOK AWAY THIS IS SOME 400 LEVEL s**t.
void AAuyron::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveX", this, &AAuyron::MoveRight);
	InputComponent->BindAxis("MoveY", this, &AAuyron::MoveForward);
	InputComponent->BindAxis("CameraPitch", this, &AAuyron::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AAuyron::YawCamera);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AAuyron::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AAuyron::Unjump);
	InputComponent->BindAction("CameraFaceForward", IE_Pressed, this, &AAuyron::CameraFaceForward);
}

// Can you believe the tutorial wanted me to use Y for horizontal movement
void AAuyron::MoveRight(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

// and X for vertical? I mean who does that?
void AAuyron::MoveForward(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AAuyron::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AAuyron::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AAuyron::Jump()
{
	if (OnTheGround) {
		JumpNextFrame = true;
		HoldingJump = true;
	}
}

// I wish you could unjump in real life
void AAuyron::Unjump()
{
	HoldingJump = false;
}

// HEY LINK TALK TO ME USING Z TARGETING
void AAuyron::CameraFaceForward()
{
	FRotator NewRotation = SpringArm->GetComponentRotation();
	NewRotation.Yaw = PlayerModel->GetComponentRotation().Yaw;
	SpringArm->SetRelativeRotation(NewRotation);
}