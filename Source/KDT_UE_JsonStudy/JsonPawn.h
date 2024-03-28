// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JsonPawn.generated.h"

USTRUCT(BlueprintType)
struct FSchoolData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString school;
	UPROPERTY()
	FString department;
	UPROPERTY()
	FString gender;
	UPROPERTY()
	int32 numberOfClasses;
};

USTRUCT(BlueprintType)
struct FStudyInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString id;
	UPROPERTY()
	FString shortname;
	UPROPERTY()
	FString url;
	UPROPERTY()
	FString course_image;
	UPROPERTY()
	FString org;
	UPROPERTY()
	FString org_name;
	UPROPERTY()
	FString enrollment_start;
	UPROPERTY()
	FString enrollment_end;
	UPROPERTY()
	FString study_start;
	UPROPERTY()
	FString study_end;
	UPROPERTY()
	FString professor;
	UPROPERTY()
	FString public_yn;
};

USTRUCT(BlueprintType)
struct FStudyInfoList
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FStudyInfo> items;
};




USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 price;
	UPROPERTY()
	FString name;
};

USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString name;
	UPROPERTY()
	int32 age;
	UPROPERTY()
	float height;
	UPROPERTY()
	bool gender; // true : 여성, false : 남성
	UPROPERTY()
	TArray<FString> interests;
	UPROPERTY()
	FItemData itemData;
};

UENUM(BlueprintType)
enum class EType : uint8
{
	AAA,
	BBB,
	MAX
};


UCLASS()
class KDT_UE_JSONSTUDY_API AJsonPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AJsonPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	void DataToJsonExample();
	void JsonToDataExample();
	void CSVToDataExample();
	void GetTest();
	void OnHttpResponse(TSharedPtr<class IHttpRequest> request, TSharedPtr<class IHttpResponse> response, bool bConnectedSuccessfully);
};
