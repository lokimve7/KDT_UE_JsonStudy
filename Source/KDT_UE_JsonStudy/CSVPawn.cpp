// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVPawn.h"
#include "CSVLoader.h"

// Sets default values
ACSVPawn::ACSVPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACSVPawn::BeginPlay()
{
	Super::BeginPlay();
	
	CSVLoader::CSVToData(TEXT("StudentInfo.csv"));
}

// Called every frame
void ACSVPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSVPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

