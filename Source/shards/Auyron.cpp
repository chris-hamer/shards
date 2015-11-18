// DON'T TELL ME WHERE TO FILL OUT MY COPYRIGHT NOTICE YOU'RE NOT MY REAL DAD.

#include "shards.h"
#include "Gem.h"
#include "Auyron.h"
#include "AuyronMovementComponent.h"
#include "CameraOverrideRegion.h"
#include "EngineUtils.h" 
#include "Stick.h"  
#include "TeleClaw.h"

// Sets default values
AAuyron::AAuyron()
{	
	// These should work.
	GroundAccelerationRate = 5500.0f;
	AirAccelerationRate = 850.0f;
	GroundDeceleration = 400.0;
	AirDeceleration = 50.0f;
	MaxVelocity = 500.0f;
	DashSpeed = 1500.0f;
	DashDuration = 0.25f;
	MaxSlope = 45.0f;
	TurnRate = 480.0f;
	JumpPower = 450.0f;
	OffGroundJumpTime = 0.04f;
	Gravity = 900.0f;
	UnjumpRate = 1.5f;
	FacingAngleSnapThreshold = 5.0f;
	TeleportAngleTolerance = 5.0f;
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
	CapsuleComponent->InitCapsuleSize(45.0f, 90.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuyron::Hit);
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AAuyron::Stay);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AAuyron::UnHit);
	SetActorEnableCollision(true);

	// It you.
	PlayerModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualRepresentation"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshObj(TEXT("/Game/Models/Characters/Auyron/Auyron"));
	PlayerModel->SetSkeletalMesh(PlayerMeshObj.Object);
	PlayerModel->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	PlayerModel->AttachTo(RootComponent);

	SidewaysSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SidewaysSpringArm"));
	SidewaysSpringArm->AttachTo(RootComponent);

	// Use a spring arm so the camera can be all like swoosh.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 60.0f), FRotator(-50.0f, 0.0f, 0.0f));
	SpringArm->bEnableCameraLag = true;

	// Camera so the casuals can "see what they're doing" or whatever.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	DashParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Dash Particles"));
	const ConstructorHelpers::FObjectFinder<UParticleSystem> dp(TEXT("/Game/Textures/Characters/Auyron/DashParticles"));
	DashParticles->SetTemplate(dp.Object);
	DashParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	DashParticles->AttachTo(PlayerModel);

	// ASSUMING DIRECT CONTROL.
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Apparently we need some newfangled "MovementComponent".
	MovementComponent = CreateDefaultSubobject<UAuyronMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
}
void AAuyron::UnHit(class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (OtherActor->IsA(ACameraOverrideRegion::StaticClass())) {

			if (CameraOverrideLookAtPlayer == ((ACameraOverrideRegion*)OtherActor)->LookAtPlayer &&
				CameraOverrideTargetDisplacement == ((ACameraOverrideRegion*)OtherActor)->TargetDisplacement &&
				CameraOverrideTargetRotation == ((ACameraOverrideRegion*)OtherActor)->TargetRotation) {
				cameralocked = false;
				CameraOverrideLookAtPlayer = false;
				CameraOverrideTargetDisplacement = FVector::ZeroVector;
				CameraOverrideTargetRotation = FRotator::ZeroRotator;
			}
		}
	}
}
void AAuyron::Stay(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (OtherActor->IsA(ACameraOverrideRegion::StaticClass())) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "asdfasfdsafdsafdasdf");
			cameralocked = true;
			CameraOverrideLookAtPlayer = ((ACameraOverrideRegion*)OtherActor)->LookAtPlayer;
			CameraOverrideTargetDisplacement = ((ACameraOverrideRegion*)OtherActor)->TargetDisplacement;
			CameraOverrideTargetRotation = ((ACameraOverrideRegion*)OtherActor)->TargetRotation;
		}
	}
}
void AAuyron::Hit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->IsA(AGem::StaticClass()))
		{
			OtherActor->Destroy();
			GemCount++;
		}
		if (OtherActor->IsA(ACameraOverrideRegion::StaticClass())) {
			cameralocked = true;
			CameraOverrideLookAtPlayer = ((ACameraOverrideRegion*)OtherActor)->LookAtPlayer;
			CameraOverrideTargetDisplacement = ((ACameraOverrideRegion*)OtherActor)->TargetDisplacement;
			CameraOverrideTargetRotation = ((ACameraOverrideRegion*)OtherActor)->TargetRotation;
		}
	}
}

// GAME START.
void AAuyron::BeginPlay()
{
	Super::BeginPlay();
	SidewaysSpringArm->TargetArmLength = 0.0f;
	SpringArm->TargetArmLength = DefaultArmLength;
	SpringArm->CameraLagSpeed = CameraLag;
	SpringArm->CameraRotationLagSpeed = 0.0f;
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
	DashParticles->DeactivateSystem();

	if (asd) {
		// Create the widget and store it.
		thehud = CreateWidget<UUserWidget>(GetWorld(), asd);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (thehud)
		{
			//let add it to the view port
			thehud->AddToViewport();
		}
	}
}

// Called every frame UNLIKE UNITY MIRITE?
void AAuyron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set our frame of reference to be that of the surface we're standing on.
	Velocity -= MovementComponent->groundvelocity;

	// Update timing values.
	TimeSinceLastMouseInput += DeltaTime;

	JustJumped = false;

	bool cameralocktemp = cameralocked;

	if (cameramode) {
		cameralocked = false;
	}

	// Move the camera in response to mouse movement.
	if(!cameralocked||ztarget) {
		// Reset camera input timer if the camera controls were touched.
		if (CameraInput.X != 0.0f || CameraInput.Y != 0.0f) {
			TimeSinceLastMouseInput = 0.0f;
		}

		FRotator NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Yaw = NewRotation.Yaw + CameraInput.X;

		// The camera should only turn with the player if the mouse hasn't been touched recently.
		if (TimeSinceLastMouseInput > CameraResetTime && !ztarget) {
			NewRotation.Yaw = NewRotation.Yaw + MovementInput.X*CameraAutoTurnFactor;
		}
		SpringArm->SetWorldRotation(NewRotation);

		NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -CameraMaxAngle, -CameraMinAngle);
		SpringArm->SetWorldRotation(NewRotation);
		SpringArm->CameraRotationLagSpeed = 0.0f;
	} else {
		SpringArm->SetRelativeRotation(CameraOverrideTargetRotation);
		SpringArm->SetRelativeLocation(CameraOverrideTargetDisplacement);
		if (CameraOverrideLookAtPlayer) {
			SpringArm->SetRelativeRotation((-SpringArm->RelativeLocation).Rotation());
		}
		SpringArm->CameraRotationLagSpeed = CameraLag;
		SpringArm->TargetArmLength = (SpringArm->GetComponentLocation() - GetActorLocation()).Size();
	}

	if (ztarget) {
		FRotator NewRotation = PlayerModel->GetComponentRotation();
		NewRotation.Yaw += CameraInput.X;
		TargetDirection.Yaw = NewRotation.Yaw;
		PlayerModel->SetWorldRotation(NewRotation);
	}


	// Move the player in response to movement inputs.
	{

		if (ztarget&&!dashing) {

			// Set the spring arm's new rotation and remove its lag.
			FRotator NewRotation = SpringArm->RelativeRotation;
			if (!wasztarget) {
				if (cameralocked) {
					NewRotation.Pitch = 0.0f;
				}
				if (MovementInput.IsNearlyZero()) {
					NewRotation.Yaw = TargetDirection.Yaw;
				} else {
					NewRotation.Yaw = SpringArm->GetComponentRotation().Yaw +
						(MovementInput.X >= 0 ? 1 : -1) * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
				}
			}
			PlayerModel->RelativeRotation.Yaw = NewRotation.Yaw;
			SpringArm->SetRelativeRotation(NewRotation);
			SpringArm->TargetArmLength = 300.0f;
			SpringArm->CameraLagSpeed = 0.0f;

			// Get the camera's right and forward vectors and transform them from world to realtive vectors.
			FVector Right = FVector::VectorPlaneProject(CapsuleComponent->GetComponentRotation().RotateVector(Camera->GetRightVector()), FVector::UpVector);
			FVector Forward = FVector::VectorPlaneProject((-(CapsuleComponent->GetComponentRotation())).RotateVector(Camera->GetForwardVector()), FVector::UpVector);

			// Move the spring arm.
			//FVector base = FVector(0.0f, 0.0f, 50.0f) + FVector(-100.0f, -100.0f, 50.0f);
			FVector base = FVector(0.0f, 0.0f, 50.0f) + FVector(0.0f, 100.0f, 50.0f);
			base = (base.X*Forward + base.Y*Right + base.Z*FVector::UpVector);
			//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, base.ToString());

			//SpringArm->SetRelativeLocation(FVector::ZeroVector);
			//SpringArm->SetRelativeLocation(FVector(0.0f, -100.0f, 50.0f));
			SpringArm->SetRelativeLocation(base);
			movementlocked = true;
		} else {
			// Return the spring arm to its original location.
			if (!cameralocked) {
				SpringArm->TargetArmLength = DefaultArmLength;
				SpringArm->CameraLagSpeed = CameraLag;
				SpringArm->CameraRotationLagSpeed = 0.0f;
				SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
			}
			movementlocked = false;
		}
		cameralocked = cameralocktemp;

		if (swish) {
			// Iterate over each TelePad and cast a ray.
			AStick* closest = NULL;
			float biggestdot = -1.0f;
			for (TActorIterator<AStick> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
				if (ActorItr->GetClass()->GetName() == "Stick") {
					//FVector displacement = ActorItr->GetActorLocation() - GetActorLocation();
					FVector displacement = ActorItr->GetActorLocation() - Camera->GetComponentLocation();

					float dot = displacement.GetSafeNormal() | Camera->GetForwardVector().GetSafeNormal();
					FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, *ActorItr);
					TraceParams.bTraceComplex = true;

					// Don't want the ray to collide with the player model now do we?
					TraceParams.AddIgnoredActor(*ActorItr);
					TraceParams.AddIgnoredActor(this);
					FHitResult f;
					FCollisionObjectQueryParams asdf = FCollisionObjectQueryParams(ECC_WorldStatic);

					// Figure out if the ray is blocked by an object.
					bool blocked = GetWorld()->LineTraceSingle(f, Camera->GetComponentLocation(), ActorItr->GetActorLocation(), TraceParams, asdf);
					if (dot > biggestdot && !blocked) {
						closest = *ActorItr;
						biggestdot = dot;
					}
				}
			}

			// Teleport the player to the TelePad that they are aiming at.
			if (closest != nullptr &&
				biggestdot > FMath::Cos(FMath::DegreesToRadians(TeleportAngleTolerance)) &&
				(closest->GetActorLocation() - GetActorLocation()).Size() < TeleportRange &&
				ztarget) {
				AStick* s = closest;
				SetActorLocation(s->gohere);
				Velocity += s->PostTeleportVelocity;
				ztarget = false;
				OnTheGround = false;
				WasOnTheGround = false;
			}
			swish = false;
		}

		if (dashing) {
			movementlocked = true;
		}

		// If you can't move, don't move.
		if (movementlocked) {
			MovementInput = FVector::ZeroVector;
			if (!dashing) {
				JumpNextFrame = false;
			}
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
		FVector Acceleration = FVector::ZeroVector;
		Acceleration = (Right*AdjustedInput.X + Forward*AdjustedInput.Y)*(OnTheGround ? GroundAccelerationRate : AirAccelerationRate);
		
		// Apply deceleration.
		Acceleration -= (FVector::VectorPlaneProject(Velocity, FVector(0.0f, 0.0f, 1.0f)))*(OnTheGround ? GroundDeceleration : AirDeceleration)*DeltaTime;

		// Ask the movement component if we're on the ground and apply gravity if we aren't.
		OnTheGround = MovementComponent->onground;
		if (!OnTheGround) {
			Acceleration += FVector(0.0f,0.0f,Gravity);
		} else if(WasOnTheGround) {
			// Push the player into the ground a bit so we still get collisions every frame.
			// Also keeps player from falling off slopes while running down them.
			if (MovementComponent->Floor.Normal.Z > 0.0f) {
				Velocity.Z = -100.0f;
			}
		}

		// You done bonked yer head on that there ceiling.
		if (MovementComponent->Floor.Normal.Z < -0.6f) {
			Velocity.Z = 0.0f;
		}

		// Physiiiiicccss.
		Velocity += Acceleration * DeltaTime;

		// Only cap the horizontal movement velocity.
		FVector temp;
		temp = Velocity;
		Velocity.Z = 0.0f;
		Velocity = Velocity.GetClampedToMaxSize(MaxVelocity);
		Velocity.Z = temp.Z;

		// Handle jumping.
		if (JumpNextFrame) {
			// Jump taking the floor's angle and vertical movement into account.
			Velocity.Z = 0.5f * (MovementComponent->groundvelocity.Z >= 0.0f ? MovementComponent->groundverticalvelocity : -MovementComponent->groundvelocity.Z);
			Velocity += JumpPower * (MovementComponent->offGroundTime > 0 ? FVector::UpVector : MovementComponent->Floor.Normal).GetSafeNormal();
			JumpNextFrame = false;
			WasOnTheGround = false;
			JustJumped = true;

			// No cheating.
			if (dashing) {
				DashParticles->DeactivateSystem();
				dashing = false;
				dashtimer = 0.0f;
			}

			// Don't store the player's old target direction anymore.
			TargetDirection.Yaw = PlayerModel->GetComponentRotation().Yaw;

			// Wall jump.
			FVector wnproject = FVector::VectorPlaneProject(MovementComponent->wallnormal, FVector::UpVector);
			if (!OnTheGround && wnproject.Size()>0.9f) {
				FRotator newmodelrotation = PlayerModel->GetComponentRotation();
				newmodelrotation.Yaw = (MovementComponent->wallnormal.GetSafeNormal() + FVector::VectorPlaneProject((Right*AdjustedInput.X + Forward*AdjustedInput.Y).GetSafeNormal(), MovementComponent->wallnormal.GetSafeNormal())).Rotation().Yaw;
				PlayerModel->SetWorldRotation(newmodelrotation);
				TargetDirection = newmodelrotation;
				temp = Velocity;
				Velocity = (MovementComponent->wallnormal.GetSafeNormal()*MaxVelocity + FVector::VectorPlaneProject((Right*AdjustedInput.X + Forward*AdjustedInput.Y), MovementComponent->wallnormal.GetSafeNormal())*MaxVelocity);
				Velocity.Z = temp.Z;
			}
		}

		// Variable jump height
		if (!HoldingJump && Velocity.Z > 0) {
			Velocity += Gravity * FVector(0, 0, UnjumpRate) * DeltaTime;
		}

		if (dash) {
			dash = false;
			if (OnTheGround) {
				DashParticles->ActivateSystem();
				dashing = true;
			}
		}

		if (dashing) {
			float z = Velocity.Z;
			Velocity = TargetDirection.Vector().GetSafeNormal()*1500.0f;
			Velocity.Z = z;
			dashtimer += DeltaTime;
			if (dashtimer > DashDuration) {
				dashing = false;
				DashParticles->DeactivateSystem();
				dashtimer = 0.0f;
			}
		}

		// Put Velocity back in the reference frame of the stationary world.
		Velocity += MovementComponent->groundvelocity;

		// Tad's Shame
		//WasOnTheGround = !WasOnTheGround && (MovementComponent->offGroundTime < OffGroundJumpTime ? false : OnTheGround);

		// Store current on the ground state into WasOnTheGround.
		WasOnTheGround = OnTheGround;

		wasztarget = ztarget;

		// And now we get to actually move.
		MovementComponent->AddInputVector(Velocity * DeltaTime);

		// If we're trying to move, take the camera's orientation into account to figure
		// out the direction we want to face.
		if (!ztarget && OnTheGround && !JustJumped) {
			// I'ma tell ya not even Unity was stupid enough to use -180 -> 180 for rotations.
			int8 reflect = (MovementInput.X >= 0 ? 1 : -1);

			// (but I forgive you)
			if (!MovementInput.IsNearlyZero() && !ztarget) {
				TargetDirection.Yaw = SpringArm->GetComponentRotation().Yaw +
					reflect * FMath::RadiansToDegrees(FMath::Acos(MovementInput.GetSafeNormal() | FVector(0, 1, 0)));
			}
		}

		// Turn to face the target direction, but do it all smooth like.
		{
			// Horrible quaternion voodoo. Viewer discretion is advised.
			// I'm honestly still not quite sure what I did.
			FQuat test = FQuat::FindBetween(PlayerModel->GetComponentRotation().Vector(), TargetDirection.Vector());
			float angle = 0.0f;
			FVector dummy;
			test.ToAxisAndAngle(dummy, angle);
			
			float multiply = (dashing ? 3.0f : 1.0f);

			// Snap to the target angle if we're close enough, otherwise just keep turning.
			if (!ztarget&&OnTheGround) {
				if (FMath::Abs(angle) > FMath::DegreesToRadians(multiply*FacingAngleSnapThreshold)) {
					test = FQuat(dummy, FMath::DegreesToRadians(multiply*TurnRate)*DeltaTime);
					PlayerModel->AddLocalRotation(test);
				} else {
					PlayerModel->SetWorldRotation(TargetDirection);
				}
			}
		}
		// Like what even ARE quaternions anyway?
	}
}

// Bind buttons and axes to movement functions.
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
	InputComponent->BindAction("CameraFaceForward", IE_Released, this, &AAuyron::CameraUnFaceForward);
	InputComponent->BindAction("CameraMode", IE_Pressed, this, &AAuyron::CameraModeToggle);
	InputComponent->BindAction("Warp", IE_Pressed, this, &AAuyron::Warp);
	InputComponent->BindAction("Dash", IE_Pressed, this, &AAuyron::Dash);
	InputComponent->BindAction("Dash", IE_Released, this, &AAuyron::UnDash);
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
	FVector wnproject = FVector::VectorPlaneProject(MovementComponent->wallnormal, FVector::UpVector);
	if (OnTheGround || MovementComponent->offGroundTime < OffGroundJumpTime || (!MovementComponent->wallnormal.IsNearlyZero() && wnproject.Size()>0.9f)) {
		JumpNextFrame = true;
		HoldingJump = true;
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
	if (!dashing) {
		ztarget = true;
	}
}

void AAuyron::CameraUnFaceForward()
{
	if (!dashing) {
		ztarget = false;
	}
}

void AAuyron::CameraModeToggle()
{
	cameramode = !cameramode;
}

// swish
void AAuyron::Warp()
{
	swish = true;
}

// woosh
void AAuyron::Dash()
{
	if (!dashing&&!ztarget) {
		dash = true;
	}
}

void AAuyron::UnDash()
{
	if (dashing) {
		dashtimer = DashDuration + 100.0f;
	}
}

float AAuyron::GetSpeed()
{
	return (FVector::VectorPlaneProject(Velocity - MovementComponent->groundvelocity, FVector::UpVector)).Size();
}

//int AAuyron::GetGemCount() {
//	return GemCount;
//}

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