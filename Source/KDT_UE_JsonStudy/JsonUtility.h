// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonUtility.generated.h"

/**
 * 
 */
UCLASS()
class KDT_UE_JSONSTUDY_API UJsonUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	/*
	UPROPERTY()
	int32 price = 1000;
	UPROPERTY()
	FString name = "포션";
	*/

public:
	template<typename T>
	static FString DataToJson(void* data)
	{
		// T 의 자료형이 어떤 구조로 되어있는지 
		UScriptStruct* p = T::StaticStruct();

		// data 를 JsonObject 로 담을 변수
		TSharedPtr<FJsonObject> jsonObject = MakeShared<FJsonObject>();

		// FProperty : 변수에 대한 정보
		for (FProperty* property = p->PropertyLink; property != nullptr; property = property->PropertyLinkNext)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Varible Name : %s"), *property->GetName());

			// 변수 이름
			FString propertyName = property->GetName();

			// 변수의 값에 접근
			if (property->IsA<FStrProperty>())
			{
				// data 에 propertyName 으로 되어있는 변수의 주소값
				FString* valuePtr = property->ContainerPtrToValuePtr<FString>(data);

				// propertyName 으로 되어있는 변수에 값을 가져와서 JsonObject 에 셋팅
				jsonObject->SetStringField(propertyName, *valuePtr);

				//UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *propertyName, **valuePtr);
			}
			else if (property->IsA<FIntProperty>())
			{
				int32* valuePtr = property->ContainerPtrToValuePtr<int32>(data);
				jsonObject->SetNumberField(propertyName, *valuePtr);
				//UE_LOG(LogTemp, Warning, TEXT("%s : %d"), *propertyName, *valuePtr);
			}
			else if (property->IsA<FFloatProperty>())
			{
				float* valuePtr = property->ContainerPtrToValuePtr<float>(data);
				jsonObject->SetNumberField(propertyName, *valuePtr);
				//UE_LOG(LogTemp, Warning, TEXT("%s : %f"), *propertyName, *valuePtr);
			}
			else if (property->IsA<FBoolProperty>())
			{
				bool* valuePtr = property->ContainerPtrToValuePtr<bool>(data);
				jsonObject->SetBoolField(propertyName, *valuePtr);
				//UE_LOG(LogTemp, Warning, TEXT("%s : %d"), *propertyName, *valuePtr);
			}
			else if (property->IsA<FArrayProperty>())
			{
				auto value = ArrayToJsonArray(property, data);
				jsonObject->SetArrayField(propertyName, value);
			}
			else if(property->IsA<FStructProperty>())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s : etc..."), *propertyName);
			}
		}

		// JsonObject 를 String으로 변환해서 반환
		return JsonToString(jsonObject);
	}

	static TArray<TSharedPtr<FJsonValue>> ArrayToJsonArray(FProperty* property, void* data)
	{
		TArray<TSharedPtr<FJsonValue>> jsonArray;

		FArrayProperty* arrayProperty = CastField<FArrayProperty>(property);
		// Array 가 어떤 타입을 들고 있는지 정보
		FProperty* innerProperty = arrayProperty->Inner;
		// data 의 Array 대한 주소값을 가져오자
		void* arrayValuePtr = arrayProperty->ContainerPtrToValuePtr<void>(data);
		// array 의 값을 접근하기 위한 Helper
		FScriptArrayHelper arrayHelper(arrayProperty, arrayValuePtr);

		// 만약에 innerProperty 가 String 이라면
		if (innerProperty->IsA<FStrProperty>())
		{
			SetArrayValue<FString, FJsonValueString>(arrayHelper, jsonArray);
		}
		else if (innerProperty->IsA<FIntProperty>())
		{
			SetArrayValue<int32, FJsonValueNumber>(arrayHelper, jsonArray);
		}

		return jsonArray;
	}

	template<typename T, typename U>
	static void SetArrayValue(FScriptArrayHelper& arrayHelper, TArray<TSharedPtr<FJsonValue>>& jsonArray)
	{
		for (int32 i = 0; i < arrayHelper.Num(); i++)
		{
			T value = *(T*)arrayHelper.GetRawPtr(i);
			jsonArray.Add(MakeShared<U>(value));
		}
	}

	static FString JsonToString(TSharedPtr<FJsonObject> jsonObject)
	{
		FString jsonString = "";
		auto jsonWriter = TJsonWriterFactory<>::Create(&jsonString);
		FJsonSerializer::Serialize(jsonObject.ToSharedRef(), jsonWriter);
		return jsonString;
	}
};
