// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "DialogueCut.h"


// Sets default values
ADialogueCut::ADialogueCut()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CutDuration = 0.0f;
	cuttimer = -1.0f;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(Root);
	//Camera->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);//4.12
}

// Called when the game starts or when spawned
void ADialogueCut::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADialogueCut::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (cuttimer >= 0.0f) {
		cuttimer += DeltaTime;
	}
}

void ADialogueCut::SetText(FString text) {
	DialogueText = text;
}

void ADialogueCut::SetNext(ADialogueCut* nextcut) {
	Next = nextcut;
}
