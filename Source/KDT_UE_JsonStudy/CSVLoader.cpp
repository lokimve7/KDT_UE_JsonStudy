// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVLoader.h"
#include "CSVPawn.h"

CSVLoader::CSVLoader()
{
}

CSVLoader::~CSVLoader()
{
}

TArray<FStudentInfo> CSVLoader::CSVToData(FString filename)
{
	// csv 파일 경로 만들기
	FString path = FPaths::ProjectDir() + TEXT("CSV/") + filename;
	UE_LOG(LogTemp, Warning, TEXT("file path : %s"), *path);

	// 모든 학생정보 담을 변수
	TArray<FStudentInfo> studentInfos;

	// 파일 존재 하지 않으면 함수를 나가자
	if (FPaths::FileExists(path) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("file not exist : %s"), *path);
		return studentInfos;
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
			// 값을 , 기준으로 나누자
			TArray<FString> values;
			rows[i].ParseIntoArray(values, TEXT(","));

			// 나눈 값을 자료형에 맞게 대입하자
			FStudentInfo info;
			for (int32 j = 0; j < variableNames.Num(); j++)
			{
				if (variableNames[j] == TEXT("Name"))
				{
					info.name = values[j];
				}
				else if (variableNames[j] == TEXT("Age"))
				{
					info.age = FCString::Atoi(*values[j]);
				}
				else if (variableNames[j] == TEXT("Gender"))
				{
					info.gender = values[j].ToBool();
				}
				else if (variableNames[j] == TEXT("StudentId"))
				{
					info.studentId = values[j];
				}
			}

			// 학생 정보 하나 추가
			studentInfos.Add(info);
		}
	}
	// 모든학생 정보 반환
	return studentInfos;
}
