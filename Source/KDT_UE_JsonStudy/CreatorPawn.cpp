// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatorPawn.h"

// Sets default values
ACreatorPawn::ACreatorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACreatorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACreatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACreatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACreatorPawn::CreateCube()
{
	GetWorld()->SpawnActor<AActor>(cubeFactory);
}

void ACreatorPawn::CreateSphere()
{
	GetWorld()->SpawnActor<AActor>(sphereFactory);
}

void ACreatorPawn::CreateCone()
{
	GetWorld()->SpawnActor<AActor>(coneFactory);
}

void ACreatorPawn::CreateShape(int32 shapeIdx)
{
	FVector randLocation = GetRandLocation();

	AActor* shape = GetWorld()->SpawnActor<AActor>(shapeFactory[shapeIdx]);
	shape->SetActorLocation(randLocation);
}

FVector ACreatorPawn::GetRandLocation()
{
	//랜덤 위치 구하자
	float randX = FMath::RandRange(-500.0f, 500.0f);
	float randY = FMath::RandRange(-500.0f, 500.0f);
	float randZ = FMath::RandRange(-500.0f, 500.0f);

	return FVector(randX, randY, randZ);
}

