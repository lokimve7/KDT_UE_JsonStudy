// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JsonPawn.generated.h"

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
	TArray<int32> randNum;
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

public:
	void RequestShortTermForecast();
	void OnCompleteRequest(TSharedPtr<class IHttpRequest> Request, 
							TSharedPtr<class IHttpResponse> Response, 
							bool bConnectedSuccessfully);
};
