// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class KDT_UE_JSONSTUDY_API CSVLoader
{
public:
	CSVLoader();
	~CSVLoader();

	template<typename T>
	static TArray<T> CSVToData(FString filename)
	{
		// csv 파일 경로 만들기
		FString path = FPaths::ProjectDir() + TEXT("CSV/") + filename;
		UE_LOG(LogTemp, Warning, TEXT("file path : %s"), *path);

		// 모든 정보 담을 변수
		TArray<T> infos;

		// 파일 존재 하지 않으면 함수를 나가자
		if (FPaths::FileExists(path) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("file not exist : %s"), *path);
			return infos;
		}

		// csv 파일 읽어 오기
		FString fileContent;
		if (FFileHelper::LoadFileToString(fileContent, *path))
		{
			UE_LOG(LogTemp, Warning, TEXT("file content : %s"), *fileContent);
			// 한줄 한줄 나누기
			TArray<FString> rows;
			fileContent.ParseIntoArrayLines(rows);

			// 변수 이름 나누기
			TArray<FString> variableNames;
			rows[0].ParseIntoArray(variableNames, TEXT(","));
			for (int32 i = 0; i < variableNames.Num(); i++)
				variableNames[i].RemoveSpacesInline();

			// 값들을 나누기

			for (int32 i = 1; i < rows.Num(); i++)
			{
				// 값을 , 기준으로 나누자
				TArray<FString> values;
				rows[i].ParseIntoArray(values, TEXT(","));
				if (values.Num() != variableNames.Num())
				{
					UE_LOG(LogTemp, Error, TEXT("value contain , %d"), i);
					continue;
				}

				// 나눈 값을 자료형에 맞게 대입하자
				T info;
				
				// T 의 자료형이 어떤 구조로 되어있는지 
				UScriptStruct* p = T::StaticStruct();

				for (int32 j = 0; j < variableNames.Num(); j++)
				{
					// variableNames[j] 이름으로 되어있는 변수의 정보
					FProperty* property = p->FindPropertyByName(*variableNames[j]);

					if (property == nullptr)
					{
						UE_LOG(LogTemp, Error, TEXT("proprety is null : %s"), *variableNames[j]);
						continue;
					}

					if (property->IsA<FStrProperty>())
					{
						FString* valuePtr = property->ContainerPtrToValuePtr<FString>(&info);
						*valuePtr = values[j];
					}
					else if (property->IsA<FIntProperty>())
					{
						int32* valuePtr = property->ContainerPtrToValuePtr<int32>(&info);
						*valuePtr = FCString::Atoi(*values[j]);
					}
					else if (property->IsA<FBoolProperty>())
					{
						bool* valuePtr = property->ContainerPtrToValuePtr<bool>(&info);
						*valuePtr = values[j].ToBool();
					}
					else if (property->IsA<FFloatProperty>())
					{
						float* valuePtr = property->ContainerPtrToValuePtr<float>(&info);
						*valuePtr = FCString::Atof(*values[j]);
					}
				}

				// 정보 하나 추가
				infos.Add(info);
			}
		}
		// 모든 정보 반환
		return infos;
	}

	static TArray<struct FStudentInfo> CSVToData(FString filename);
};
