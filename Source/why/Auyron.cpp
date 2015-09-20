// DON'T TELL ME WHERE TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD.

#include "why.h"
#include "Gem.h"
#include "Auyron.h"
#include "AuyronMovementComponent.h"
#include "EngineUtils.h" 
#include "Stick.h"  
#include "TeleClaw.h" 

// Sets default values
AAuyron::AAuyron()
{
	// Gonna use these to flag abilities
	HasTeleport = true;
	MaxExtraJumps = 1;
	jumpsLeft = MaxExtraJumps;
	HasGlide = false; // false because I haven't programmed this yet

	// These should work.
	AccelerationRate = 5500.0f;
	GroundDeceleration = 600.0f;
	AirDeceleration = 50.0f;
	MaxVelocity = 470.0f;
	MaxSlope = 45.0f;
	TurnRate = 500.0f;
	JumpPower = 400.0f;
	DoubleJumpPower = 350.0f;
	OffGroundJumpTime = 0.08f;
	Gravity = 1000.0f;
	UnjumpRate = 1.5f;
	FacingAngleSnapThreshold = 5.0f;
	TeleportAngleTolerance = 20.0f;
	TeleportRange = 1400.0f;
	CameraMaxAngle = 85.0f;
	CameraMinAngle = -85.0f;
	DefaultArmLength = 400.0f;
	CameraLag = 3.0f;
	CameraAutoTurnFactor = 1.0f;
	CameraResetTime = 1.0f;

	// Just in case.
	TargetDirection = FRotator::ZeroRotator;
	Velocity = FVector::ZeroVector;

	// Oh you better believe we're ticking every frame.
	PrimaryActorTick.bCanEverTick = true;

	// I wanted to be a cylinder, but no, we gotta be a capsule.
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->InitCapsuleSize(25.0f, 90.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	SetActorEnableCollision(true);
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AAuyron::HitGem);

	// It you.
	PlayerModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualRepresentation"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Game/Models/Characters/Auyron/Auyron"));
	PlayerModel->SetSkeletalMesh(MeshObj.Object);
	PlayerModel->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	//PlayerModel->SetRelativeScale3D(FVector(35.0f, 35.0f, 17.16083f));
	PlayerModel->AttachTo(RootComponent);

	// Use a spring arm so the camera can be all like swoosh.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 60.0f), FRotator(-50.0f, 0.0f, 0.0f));
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
	GemCount = 0;

	// FIRMLY GRASP IT IN YOUR HAND.
	ATeleClaw* tc = NULL;
	for (TActorIterator<ATeleClaw> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		tc = *ActorItr;
	}
	if (tc != nullptr) {
		PlayerModel->GetSocketByName("RightHand")->AttachActor(tc, PlayerModel);
		tc->TeleClaw->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
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
		if (ztarget) {
			NewRotation = PlayerModel->GetComponentRotation();
			NewRotation.Yaw += CameraInput.X;
			PlayerModel->SetWorldRotation(NewRotation);
		}
	}

	// Move the player in response to movement inputs.
	{

		if (ztarget) {
			//const ConstructorHelpers::FObjectFinder<UAnimSequence> aimanim(TEXT("/Game/Animations/Characters/Auyron/Auryon_Aim"));
			//PlayerModel->SetAnimation(aimanim.Object);
			FRotator NewRotation = SpringArm->GetComponentRotation();
			NewRotation.Yaw = PlayerModel->GetComponentRotation().Yaw;
			SpringArm->SetRelativeRotation(NewRotation);
			SpringArm->TargetArmLength = 100.0f;
			SpringArm->CameraLagSpeed = 0.0f;
			FVector Right = FVector::VectorPlaneProject(Camera->GetRightVector(), FVector::UpVector);
			FVector Forward = FVector::VectorPlaneProject(Camera->GetForwardVector(), FVector::UpVector);
			FVector base = FVector(0.0f, 0.0f, 50.0f) + FVector(-100.0f, -100.0f, 50.0f);
			base = (-base.X*Right + base.Y*Forward + base.Z*FVector::UpVector);
			SpringArm->SetRelativeLocation(base);
			movementlocked = true;
		} else {
			SpringArm->TargetArmLength = DefaultArmLength;
			SpringArm->CameraLagSpeed = CameraLag;
			SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
			movementlocked = false;
		}

		if (swish) {
			AStick* closest = NULL;
			float biggestdot = -1.0f;
			for (TActorIterator<AStick> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
				if (ActorItr->GetClass()->GetName() == "Stick") {
					FVector displacement = ActorItr->GetActorLocation() - GetActorLocation();

					float dot = displacement.GetSafeNormal() | Camera->GetForwardVector().GetSafeNormal();
					FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, *ActorItr);
					TraceParams.bTraceComplex = true;
					//TraceParams.bTraceAsyncScene = true;
					//TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

					//Ignore Actors
					TraceParams.AddIgnoredActor(*ActorItr);
					TraceParams.AddIgnoredActor(this);
					FHitResult f;
					FCollisionObjectQueryParams asdf = FCollisionObjectQueryParams(ECC_WorldStatic);
					//bool blocked = GetWorld()->LineTraceSingle(f, Camera->GetComponentLocation(), ActorItr->GetActorLocation(), TraceParams, asdf);
					bool blocked = GetWorld()->LineTraceSingle(f, Camera->GetComponentLocation(), ActorItr->GetActorLocation(), TraceParams, asdf);
					if (dot>biggestdot && !blocked) {
						closest = *ActorItr;
						biggestdot = dot;
					}
				}
			}
			if (closest != nullptr &&
				biggestdot > FMath::Cos(FMath::DegreesToRadians(TeleportAngleTolerance)) &&
				(closest->GetActorLocation() - GetActorLocation()).Size() < TeleportRange &&
				ztarget) {
				AStick* s = closest;
				SetActorLocation(s->gohere);
				Velocity.Z = 0.0f;
			}
			swish = false;
		}
		if (movementlocked) {
			MovementInput = FVector::ZeroVector;
			JumpNextFrame = false;
		}

		// The directions of "Right" and "Forward" depend on the direction the camera's facing.
		FVector Right = Camera->GetRightVector();
		Right.Z = 0.0f;
		Right = Right.GetSafeNormal();
		FVector Forward = Camera->GetForwardVector();
		Forward.Z = 0.0f;
		Forward = Forward.GetSafeNormal();

		// Set up acceleration vector using the movement inputs.
		FVector AdjustedInput = MovementInput.ClampSize(0.0, 1.0);
		FVector Acceleration = (Right*AdjustedInput.X + Forward*AdjustedInput.Y)*AccelerationRate;

		// Apply deceleration.
		if (OnTheGround) {
			Acceleration -= (FVector::VectorPlaneProject(Velocity, FVector(0.0f, 0.0f, 1.0f)))*GroundDeceleration*DeltaTime;
		} else {
			Acceleration -= (FVector::VectorPlaneProject(Velocity, FVector(0.0f, 0.0f, 1.0f)))*AirDeceleration*DeltaTime;
		}

		// Ask the movement component if we're on the ground
		// and apply gravity if we are.
		OnTheGround = MovementComponent->onground;
		if (OnTheGround)
		{
			jumpsLeft = MaxExtraJumps;
		}

		if (!OnTheGround) 
		{
			Acceleration += FVector(0.0f,0.0f,Gravity);
			if (WasOnTheGround && !JustJumped && MovementComponent->PlatformVelocity.SizeSquared() == 0)
			{
				Velocity.Z = 0.0f;
			}
		}

		// You done bonked yer head on that there ceiling.
		if (MovementComponent->Floor.Normal.Z < -0.6f) {
			Velocity.Z = 0.0f;
		}

		// Physiiiiicccss.
		Velocity += Acceleration * DeltaTime;
		if (Velocity.Z > 0.0f)
			JustJumped = false;

		// Only cap the horizontal movement velocity.
		float tempz;
		tempz = Velocity.Z;
		Velocity.Z = 0.0f;
		Velocity = Velocity.GetClampedToMaxSize(MaxVelocity);
		Velocity.Z = tempz;

		// Do shit for moving platforms
		//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), MovementComponent->PlatformVelocity.X, MovementComponent->PlatformVelocity.Y, MovementComponent->PlatformVelocity.Z);
		FVector loc = GetActorLocation() + MovementComponent->PlatformVelocity * DeltaTime;
		SetActorLocation(loc);

		// Handle jumping.
		if (JumpNextFrame) {
			// Jump taking the floor's angle into account.
			Velocity.Z = 0;
			// TODO: incorporate platform velocity correctly
			Velocity += (justDoubleJumped ? DoubleJumpPower : JumpPower) *
						(MovementComponent->offGroundTime > 0 || justDoubleJumped ? FVector::UpVector : MovementComponent->Floor.Normal);
			JumpNextFrame = false;
			WasOnTheGround = false;
		}

		// Variable jump height
		if (!HoldingJump && Velocity.Z > 0) {
			Velocity += Gravity * FVector(0, 0, UnjumpRate) * DeltaTime;
		}

		// Tad doesn't have shame
		//WasOnTheGround = !WasOnTheGround && (MovementComponent->offGroundTime < OffGroundJumpTime ? false : OnTheGround);

		// Store current on the ground state into WasOnTheGround.
		WasOnTheGround = OnTheGround;

		// And now we get to actually move.
		MovementComponent->AddInputVector(Velocity * DeltaTime);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::SanitizeFloat(MovementComponent->offGroundTime) + "   " + Velocity.ToString() + "   " + (OnTheGround ? "true" : "false") + "   " + (WasOnTheGround ? "true" : "false"));

		// If we're trying to move, take the camera's orientation into account to figure
		// out the direction we want to face.
		if (!ztarget) {
			// I'ma tell ya not even Unity was stupid enough to use -180 -> 180 for rotations.
			int8 reflect = (MovementInput.X >= 0 ? 1 : -1);

			// (but I forgive you)
			if (!MovementInput.IsNearlyZero() && !ztarget) {
				TargetDirection.Yaw = SpringArm->GetComponentRotation().Yaw +
					reflect * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
			}
		}

		{
			// Horrible quaternion voodoo. Viewer discretion is advised.
			// I'm honestly still not quite sure what I did.
			FQuat test = FQuat::FindBetween(PlayerModel->GetComponentRotation().Vector(), TargetDirection.Vector());
			//FQuat test = FQuat::Identity;
			float angle = 0.0f;
			FVector dummy;
			test.ToAxisAndAngle(dummy, angle);
			if (MovementInput.IsNearlyZero() && (FMath::Abs(angle) <= FMath::DegreesToRadians(FacingAngleSnapThreshold))) {
				TargetDirection = PlayerModel->GetComponentRotation();
			}
		}

		// Turn to face the target direction, but do it all smooth like.
		{
			// Horrible quaternion voodoo. Viewer discretion is advised.
			// I'm honestly still not quite sure what I did.
			FQuat test = FQuat::FindBetween(PlayerModel->GetComponentRotation().Vector(), TargetDirection.Vector());
			//FQuat test = FQuat::Identity;
			float angle = 0.0f;
			FVector dummy;
			test.ToAxisAndAngle(dummy, angle);

			// Snap to the target angle if we're close enough, otherwise just keep turning.
			if (!ztarget) {
				if (FMath::Abs(angle) > FMath::DegreesToRadians(FacingAngleSnapThreshold)) {
					test = FQuat(dummy, FMath::DegreesToRadians(TurnRate)*DeltaTime);
					PlayerModel->AddLocalRotation(test);
				} else {
					PlayerModel->SetWorldRotation(TargetDirection);
				}
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
	InputComponent->BindAction("Use", IE_Pressed, this, &AAuyron::Use);
	InputComponent->BindAction("CameraFaceForward", IE_Pressed, this, &AAuyron::CameraFaceForward);
	InputComponent->BindAction("Warp", IE_Pressed, this, &AAuyron::Warp);
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
	if (OnTheGround || MovementComponent->offGroundTime < OffGroundJumpTime) 
	{
		JustJumped = true;
		JumpNextFrame = true;
		HoldingJump = true;
		justDoubleJumped = false;
	}
	else if (jumpsLeft > 0)
	{
		JustJumped = true;
		JumpNextFrame = true;
		HoldingJump = true;
		jumpsLeft--;
		justDoubleJumped = true;
	}
}

// I wish you could unjump in real life
void AAuyron::Unjump()
{
	HoldingJump = false;
}

void AAuyron::Use()
{
	ShouldActivate = true;
}

// HEY LINK TALK TO ME USING Z TARGETING
void AAuyron::CameraFaceForward()
{
	if (HasTeleport)
	{
		ztarget = !ztarget;
	}
}

// swish
void AAuyron::Warp()
{
	swish = true;
}

void AAuyron::HitGem(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->IsA(AGem::StaticClass()))
		{
			OtherActor->Destroy();
			GemCount++;
		}
	}
}

float AAuyron::GetSpeed()
{
	return FVector::VectorPlaneProject(Velocity, FVector::UpVector).Size();
}

bool AAuyron::GetIsTurning()
{
	FQuat test = FQuat::FindBetween(PlayerModel->GetComponentRotation().Vector(), TargetDirection.Vector());
	float angle = 0.0f;
	FVector dummy;
	test.ToAxisAndAngle(dummy, angle);
	return (FMath::Abs(angle) > FMath::DegreesToRadians(FacingAngleSnapThreshold));
}

bool AAuyron::GetIsAiming()
{
	return ztarget;
}

bool AAuyron::GetIsOnTheGround()
{
	return OnTheGround;
}