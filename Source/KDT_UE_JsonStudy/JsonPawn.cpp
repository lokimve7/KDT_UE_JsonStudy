﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonPawn.h"

#include "JsonUtility.h"

// Sets default values
AJsonPawn::AJsonPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJsonPawn::BeginPlay()
{
	Super::BeginPlay();
	
	DataToJsonExample();

	//JsonToDataExample();

	/*FUserInfo userInfo;
	UJsonUtility::DataToJson<FUserInfo>(userInfo);

	FItemData itemData;
	UJsonUtility::DataToJson<FItemData>(itemData);*/
}

// Called every frame
void AJsonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AJsonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AJsonPawn::DataToJsonExample()
{
	// 가상 데이터
	FUserInfo info;
	info.name = TEXT("김현진");
	info.age = 20;
	info.height = 180;
	info.gender = false;
	info.interests = { TEXT("게임"), TEXT("돈"), TEXT("마술") };

	FString jsonString = UJsonUtility::DataToJson<FUserInfo>(&info);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *jsonString);

	FItemData itemData;
	jsonString = UJsonUtility::DataToJson<FItemData>(&itemData);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *jsonString);


	//// Data ----> JsonObject ----> JsonString
	//// 
	//// JsonObject 만들자
	//// FJsonObject* jsonObject = new FJsonObject();
	//TSharedPtr<FJsonObject> jsonObject = MakeShared<FJsonObject>();
	//jsonObject->SetStringField(TEXT("name"), info.name);
	//jsonObject->SetNumberField(TEXT("age"), info.age);
	//jsonObject->SetNumberField(TEXT("height"), info.height);
	//jsonObject->SetBoolField(TEXT("gender"), info.gender);

	//// TArray 를 jsonArray
	//TArray<TSharedPtr<FJsonValue>> jsonArray;
	//for (int32 i = 0; i < info.interests.Num(); i++)
	//{
	//	TSharedPtr<FJsonValueString> jsonValue = MakeShared<FJsonValueString>(info.interests[i]);
	//	jsonArray.Add(jsonValue);
	//}
	//jsonObject->SetArrayField(TEXT("interests"), jsonArray);

	//// JsonObject 를 FString 으로 만들자
	//FString jsonString;
	//TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&jsonString);
	//FJsonSerializer::Serialize(jsonObject.ToSharedRef(), jsonWriter);

	//UE_LOG(LogTemp, Warning, TEXT("my info : %s"), *jsonString);
}

void AJsonPawn::JsonToDataExample()
{
	FString jsonString = TEXT("{\"name\": \"김현진\",\"age\" : 20,\"height\" : 180,\"gender\" : false, \"interests\": [\"게임\",\"돈\",\"마술\"]}");
	// JsonString ----> JsonObject ----> Data
	
	// FString 을 JsonObject  로 만들자
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	TSharedPtr<FJsonObject> jsonObject;
	FJsonSerializer::Deserialize(jsonReader, jsonObject);

	// JsonObject 을 FUserInfo 로 만들자
	FUserInfo info;
	info.name = jsonObject->GetStringField(TEXT("name"));
	info.age = jsonObject->GetNumberField(TEXT("age"));
	info.height = jsonObject->GetNumberField(TEXT("height"));
	info.gender = jsonObject->GetBoolField(TEXT("gender"));

	// jsonArray 를 TArray 로 변환
	TArray<TSharedPtr<FJsonValue>> jsonArray = jsonObject->GetArrayField(TEXT("interests"));
	for (int32 i = 0; i < jsonArray.Num(); i++)
	{
		info.interests.Add(jsonArray[i]->AsString());
		UE_LOG(LogTemp, Warning, TEXT("interest : %s"), *info.interests[i]);
	}


	UE_LOG(LogTemp, Warning, TEXT("name - %s, age - %d, height - %f, gender - %d"),
			*info.name, info.age, info.height, info.gender);

}

