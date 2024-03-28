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
	
public:
	template<typename T>
	static FString DataToJson(void* data)
	{
		// T 의 자료형이 어떤 구조로 되어있는지 
		UScriptStruct* p = T::StaticStruct();

		// data 를 JsonObject 로 담을 변수
		TSharedPtr<FJsonObject> jsonObject = GetJsonObject(p, data);		

		// JsonObject 를 String으로 변환해서 반환
		return JsonToString(jsonObject);
	}

	static TSharedPtr<FJsonObject> GetJsonObject(UScriptStruct* p, void* data)
	{
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
				auto valuePtr = ArrayToJsonArray(property, data);
				jsonObject->SetArrayField(propertyName, valuePtr);
				//UE_LOG(LogTemp, Warning, TEXT("%s : Array Data"), *propertyName);
			}
			else
			{
				void* valuePtr = property->ContainerPtrToValuePtr<void>(data);
				jsonObject->SetObjectField(propertyName, GetJsonObject(property, valuePtr));
				//UE_LOG(LogTemp, Warning, TEXT("%s : etc..."), *propertyName);
			}
		}
		return jsonObject;
	}

	static TSharedPtr<FJsonObject> GetJsonObject(FProperty* property, void* data)
	{
		FStructProperty* structProperty = CastField<FStructProperty>(property);
		UScriptStruct* p = structProperty->Struct;

		return GetJsonObject(p, data);
	}

	static FString JsonToString(TSharedPtr<FJsonObject> jsonObject)
	{
		FString jsonString = "";
		auto jsonWriter = TJsonWriterFactory<>::Create(&jsonString);
		FJsonSerializer::Serialize(jsonObject.ToSharedRef(), jsonWriter);
		return jsonString;
	}

	static TSharedPtr<FJsonObject> StringToJson(FString jsonString)
	{
		TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
		TSharedPtr<FJsonObject> jsonObject;
		FJsonSerializer::Deserialize(jsonReader, jsonObject);

		return jsonObject;
	}

	static TArray<TSharedPtr<FJsonValue>> ArrayToJsonArray(FProperty* property, void* data)
	{
		TArray<TSharedPtr<FJsonValue>> jsonArray;

		FArrayProperty* arrayProperty = CastField<FArrayProperty>(property);

		// Array 가 어떤 타입을 여러개 들고 있는지 정보
		FProperty* innerProperty = arrayProperty->Inner;
		
		void* valuePtr = arrayProperty->ContainerPtrToValuePtr<void>(data);
		FScriptArrayHelper arrayHelper(arrayProperty, valuePtr);

		if (innerProperty->IsA<FStrProperty>())
		{
			for (int32 i = 0; i < arrayHelper.Num(); i++)
			{
				FString value = *(FString*)(arrayHelper.GetRawPtr(i));
				jsonArray.Add(MakeShared<FJsonValueString>(value));
				//UE_LOG(LogTemp, Warning, TEXT("%d : %s"), i, *value);
			}
		}

		return jsonArray;
	}


	template<typename T>
	static T JsonToData(FString jsonString)
	{

		TSharedPtr<FJsonObject> jsonObject = StringToJson(jsonString);

		T data;
		if (jsonObject != nullptr)
		{
			UScriptStruct* p = T::StaticStruct();

			AAA(jsonObject, p, &data);
		}

		return data;
	}

	static void AAA(TSharedPtr<FJsonObject> jsonObject, UScriptStruct* p, void* data)
	{
		for (FProperty* Property = p->PropertyLink; Property != NULL; Property = Property->PropertyLinkNext)
		{
			if (Property == nullptr) break;

			FString propertyName = *Property->GetName();

			//UE_LOG(LogTemp, Warning, TEXT("variable name : %s"), *propertyName);

			if (Property->IsA<FStrProperty>())
			{
				FString* valuePtr = Property->ContainerPtrToValuePtr<FString>(data);
				*valuePtr = jsonObject->GetStringField(propertyName);
			}
			else if (Property->IsA<FIntProperty>())
			{
				int32* valuePtr = Property->ContainerPtrToValuePtr<int32>(data);
				*valuePtr = jsonObject->GetNumberField(propertyName);
			}
			else if (Property->IsA<FArrayProperty>())
			{
				void* valuePtr = Property->ContainerPtrToValuePtr<void>(data);
				FArrayProperty* arrayProperty = CastField<FArrayProperty>(Property);
				FProperty* innerProperty = arrayProperty->Inner;
				void* arrayValuePtr = arrayProperty->ContainerPtrToValuePtr<void>(data);
				FScriptArrayHelper arrayHelper(arrayProperty, arrayValuePtr);
				
				TArray<TSharedPtr<FJsonValue>> jsonArray = jsonObject->GetArrayField(propertyName);

				if (innerProperty->IsA<FStrProperty>())
				{
					arrayHelper.AddValues(jsonArray.Num());
					for (int32 i = 0; i < jsonArray.Num(); i++)
					{
						FString* value = (FString*)arrayHelper.GetRawPtr(i);
						*value = jsonArray[i]->AsString();
					}
				}
				if (innerProperty->IsA<FStructProperty>())
				{
					FStructProperty* structProperty = CastField<FStructProperty>(innerProperty);
					arrayHelper.AddValues(jsonArray.Num());
					for (int32 i = 0; i < jsonArray.Num(); i++)
					{
						void* structValuePtr = arrayHelper.GetRawPtr(i);
						AAA(jsonArray[i]->AsObject(), structProperty->Struct, structValuePtr);
					}
				}

			}
			else if (Property->IsA<FStructProperty>())
			{
				void* valuePtr = Property->ContainerPtrToValuePtr<void>(data);
				FStructProperty* structProperty = CastField<FStructProperty>(Property);
				UScriptStruct* pp = structProperty->Struct;
				TSharedPtr<FJsonObject> jsonObject2 = jsonObject->GetObjectField(propertyName);
				AAA(jsonObject2, pp, valuePtr);
			}
		}
	}
};


/*
{
    "name": "김현진",
    "age": 20,
    "height": 180,
    "gender": false,
    "interests": [
        "게임",
        "돈",
        "마술"
    ],
    "itemData":
    {
        "price": 0,
        "name": ""
    }
}
*/