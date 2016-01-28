// Fill out your copyright notice in the Description page of Project Settings.

#include "shards.h"
#include "ShardsPlayerController.h"
#include "ShardsGameMode2.h"



void AShardsGameMode2::StartPlay()
{
	Super::StartPlay();

	StartMatch();

}