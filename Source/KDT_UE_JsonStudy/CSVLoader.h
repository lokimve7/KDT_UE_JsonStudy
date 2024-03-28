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

	static TArray<struct FStudentInfo> CSVToData(FString filename);
};
