// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CSVPawn.generated.h"

USTRUCT(BlueprintType)
struct FStudentInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FString name;
	UPROPERTY(VisibleAnywhere)
	int32 age;
	UPROPERTY(VisibleAnywhere)
	bool gender;
	UPROPERTY(VisibleAnywhere)
	FString studentId;
};

USTRUCT(BlueprintType)
struct FStopStatusInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FString name;
	UPROPERTY(VisibleAnywhere)
	int32 id;
	UPROPERTY(VisibleAnywhere)
	int32 totalNumRide;
	UPROPERTY(VisibleAnywhere)
	int32 totalNumGetOff;
	UPROPERTY(VisibleAnywhere)
	int32 numRideByCard;
	UPROPERTY(VisibleAnywhere)
	int32 numGetOffByCard;
	UPROPERTY(VisibleAnywhere)
	int32 numRideByCash;
	UPROPERTY(VisibleAnywhere)
	int32 aveNumGetOff;
};

UCLASS()
class KDT_UE_JSONSTUDY_API ACSVPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACSVPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	TArray<FStudentInfo> allStudent;

	UPROPERTY(VisibleAnywhere)
	TArray<FStopStatusInfo> allStopStstus;
};
