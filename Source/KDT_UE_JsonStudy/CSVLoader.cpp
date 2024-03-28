// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVLoader.h"

CSVLoader::CSVLoader()
{
}

CSVLoader::~CSVLoader()
{
}

void CSVLoader::CSVToData(FString filename)
{
	// csv 파일 경로 만들기
	FString path = FPaths::ProjectDir() + TEXT("CSV/") + filename;
	UE_LOG(LogTemp, Warning, TEXT("file path : %s"), *path);

	// 파일 존재 하지 않으면 함수를 나가자
	if (FPaths::FileExists(path) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("file not exist : %s"), *path);
		return;
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

		// 값들을 나누기
		for (int32 i = 1; i < rows.Num(); i++)
		{
			TArray<FString> values;
			rows[i].ParseIntoArray(values, TEXT(","));
		}
	}
}
