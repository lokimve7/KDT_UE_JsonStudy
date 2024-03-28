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
	}
}
