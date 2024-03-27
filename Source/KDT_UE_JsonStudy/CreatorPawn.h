// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CreatorPawn.generated.h"

UCLASS()
class KDT_UE_JSONSTUDY_API ACreatorPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACreatorPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 생성할 모양의 Actor 담아놀 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> cubeFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> sphereFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> coneFactory;

	// 모양들을 모두 담아 놓을 변수
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> shapeFactory;

	// 생성된 모양들을 담아놓는 변수
	UPROPERTY(VisibleAnywhere)
	TArray<class AShapeActor*> allShapes;

public:
	// 모양을 만든 함수
	UFUNCTION(BlueprintCallable)
	void CreateCube();
	UFUNCTION(BlueprintCallable)
	void CreateSphere();
	UFUNCTION(BlueprintCallable)
	void CreateCone();

	UFUNCTION(BlueprintCallable)
	void CreateShape(int32 shapeIdx);
	void CreateShape(int32 shapeIdx, FVector pos, FRotator rot, FVector scale);

	FVector GetRandLocation();
	FRotator GetRandRotation();

	// 저장 함수
	UFUNCTION(BlueprintCallable)
	void SaveData();

	// 불러오기 함수
	UFUNCTION(BlueprintCallable)
	void LoadData();


	// Cube : 0, Sphere : 1, Cone : 2
	// 1 번키 누르면 Cube 생성
	// 2 번키 누르면 Sphere 생성
	// 3 번키 누르면 Cone 생성

	// 생성된 액터를 랜덤한 위치(-10000 ~ 10000), 랜덤한 크기 (0.5 ~ 4), 랜덤한 회전

	
};
