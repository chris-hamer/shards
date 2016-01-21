// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "DialogueCut.h"
#include "NPC.h"


// Sets default values
ANPC::ANPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Custom"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	//const ConstructorHelpers::FObjectFinder<USkeletalMesh> AIMeshObj(TEXT("/Game/Models/Characters/Auyron/Auyron"));
	//GetMesh()->SetSkeletalMesh(AIMeshObj.Object);
	//GetMesh()->AddLocalOffset(FVector(0.0f, 0.0f, -90.0f));
	//const ConstructorHelpers::FObjectFinder<UAnimBlueprint> ab(TEXT("/Game/Animations/Characters/Auyron/Anim_Auyron"));
	//GetMesh()->SetAnimInstanceClass(ab.Object->GetClass());
	//GetMesh()->GlobalAnimRateScale = 2.0f;
	//const ConstructorHelpers::FClassFinder<AController> zz(TEXT("/Game/Blueprints/fangirl_controller"));
	//AIControllerClass = zz.Class;

	MaxDistance = 500.0f;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	CurrentCut = RootCut;

}

// Called every frame
void ANPC::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ANPC::Activate() {

}

ADialogueCut* ANPC::GetNextCut() {
	return CurrentCut->Next;
}

