// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatorPawn.h"
#include "ShapeActor.h"

// Sets default values
ACreatorPawn::ACreatorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACreatorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACreatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACreatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACreatorPawn::CreateCube()
{
	GetWorld()->SpawnActor<AActor>(cubeFactory);
}

void ACreatorPawn::CreateSphere()
{
	GetWorld()->SpawnActor<AActor>(sphereFactory);
}

void ACreatorPawn::CreateCone()
{
	GetWorld()->SpawnActor<AActor>(coneFactory);
}

void ACreatorPawn::CreateShape(int32 shapeIdx)
{
	FVector randLocation = GetRandLocation();
	FRotator randRotation = GetRandRotation();	
	float randScale = FMath::RandRange(0.5f, 2.0f);

	CreateShape(shapeIdx, randLocation, randRotation, FVector(randScale));
}

void ACreatorPawn::CreateShape(int32 shapeIdx, FVector pos, FRotator rot, FVector scale)
{
	AActor* shape = GetWorld()->SpawnActor<AActor>(shapeFactory[shapeIdx]);
	shape->SetActorLocation(pos);
	shape->SetActorRotation(rot);
	shape->SetActorScale3D(FVector(scale));

	// 만들어진 Shape 을 AShapeActor 로 형변환하고 allShapes 에 추가
	AShapeActor* shapeActor = Cast<AShapeActor>(shape);
	// shapeActor->type = shapeIdx; << 이미 Blueprint 에서 셋팅 해놓음
	allShapes.Add(shapeActor);	
}

FVector ACreatorPawn::GetRandLocation()
{
	//랜덤 위치 구하자
	float randX = FMath::RandRange(-500.0f, 500.0f);
	float randY = FMath::RandRange(-500.0f, 500.0f);
	float randZ = FMath::RandRange(-500.0f, 500.0f);

	return FVector(randX, randY, randZ);
}

FRotator ACreatorPawn::GetRandRotation()
{
	float randPicth = FMath::RandRange(0.0f, 360.0f);
	float randYaw = FMath::RandRange(0.0f, 360.0f);
	float randRoll = FMath::RandRange(0.0f, 360.0f);

	return FRotator(randPicth, randYaw, randRoll);
}

void ACreatorPawn::SaveData()
{
	//// JsonObject 만들자
	//TSharedPtr<FJsonObject> jsonObejct = MakeShared<FJsonObject>();
	//// JsonObject 에 allShapes 의 0번째 Actor 데이터(위치, 회전, 크기, type) 를 셋팅
	//jsonObejct->SetNumberField(TEXT("PosX"), allShapes[0]->GetActorLocation().X);
	//jsonObejct->SetNumberField(TEXT("PosY"), allShapes[0]->GetActorLocation().Y);
	//jsonObejct->SetNumberField(TEXT("PosZ"), allShapes[0]->GetActorLocation().Z);
	//jsonObejct->SetNumberField(TEXT("Pitch"), allShapes[0]->GetActorRotation().Pitch);
	//jsonObejct->SetNumberField(TEXT("Yaw"), allShapes[0]->GetActorRotation().Yaw);
	//jsonObejct->SetNumberField(TEXT("Roll"), allShapes[0]->GetActorRotation().Roll);
	//jsonObejct->SetNumberField(TEXT("Type"), allShapes[0]->type);

	//// jsonObject 를 String 으로 변환
	//FString jsonString;
	//TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&jsonString);
	//FJsonSerializer::Serialize(jsonObejct.ToSharedRef(), jsonWriter);

	//// 어디에 어떤이름으로 저장할건지
	//FString path = FPaths::ProjectContentDir() + TEXT("text.txt");
	//// path 에 jsonString 을 file 로 저장
	//FFileHelper::SaveStringToFile(jsonString, *path);


	// allShapes 에 있는 AShapeActor 의 위치,회전,크기,Type 의 정보를 Json 으로 하자
	// Data -> JsonObject -> JsonString	

	TArray<TSharedPtr<FJsonValue>> jsonArray;
	for (int32 i = 0; i < allShapes.Num(); i++)
	{
		// JsonObject 만들자
		TSharedPtr<FJsonObject> jsonObejct = MakeShared<FJsonObject>();
		// JsonObject 에 allShapes 의 0번째 Actor 데이터(위치, 회전, 크기, type) 를 셋팅
		jsonObejct->SetNumberField(TEXT("PosX"), allShapes[i]->GetActorLocation().X);
		jsonObejct->SetNumberField(TEXT("PosY"), allShapes[i]->GetActorLocation().Y);
		jsonObejct->SetNumberField(TEXT("PosZ"), allShapes[i]->GetActorLocation().Z);
		jsonObejct->SetNumberField(TEXT("Pitch"), allShapes[i]->GetActorRotation().Pitch);
		jsonObejct->SetNumberField(TEXT("Yaw"), allShapes[i]->GetActorRotation().Yaw);
		jsonObejct->SetNumberField(TEXT("Roll"), allShapes[i]->GetActorRotation().Roll);
		jsonObejct->SetNumberField(TEXT("Type"), allShapes[i]->type);
		jsonObejct->SetNumberField(TEXT("Scale"), allShapes[i]->GetActorScale3D().X);

		// jsonObject 를 jsonValue 형태로 변화 jsonArray 추가
		TSharedPtr<FJsonValueObject> valueObject = MakeShared<FJsonValueObject>(jsonObejct);
		jsonArray.Add(valueObject);
	}

	// jsonArray 를 String 으로 변환
	FString jsonString;
	TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&jsonString);
	FJsonSerializer::Serialize(jsonArray, jsonWriter);
	
	// 어디에 어떤이름으로 저장할건지
	FString path = FPaths::ProjectContentDir() + TEXT("text.txt");
	// path 에 jsonString 을 file 로 저장
	FFileHelper::SaveStringToFile(jsonString, *path);

	//UE_LOG(LogTemp, Warning, TEXT("shape : %s"), *jsonString);
}

void ACreatorPawn::LoadData()
{	
	// 불러 올 경로
	FString path = FPaths::ProjectContentDir() + TEXT("text.txt");
	// 불러 올 데이터를 담아놓을 변수
	FString jsonString;
	// jsonString 에 path 엤는 내용을 복사해 넣자
	FFileHelper::LoadFileToString(jsonString, *path);

	UE_LOG(LogTemp, Warning, TEXT("shape : %s"), *jsonString);

	// jsonString -> jsonArray
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	TArray<TSharedPtr<FJsonValue>> jsonArray;
	FJsonSerializer::Deserialize(jsonReader, jsonArray);

	for (int32 i = 0; i < jsonArray.Num(); i++)
	{
		// jsonArray 에 들어있는 값을 jsonObject 형으로 변환
		TSharedPtr<FJsonObject> jsonObject = jsonArray[i]->AsObject();
		// jsonObject 의 있는 값들을 하나하나 지역변수에 담자.
		float PosX = jsonObject->GetNumberField(TEXT("PosX"));
		float PosY = jsonObject->GetNumberField(TEXT("PosY"));
		float PosZ = jsonObject->GetNumberField(TEXT("PosZ"));
		float Pitch = jsonObject->GetNumberField(TEXT("Pitch"));
		float Yaw = jsonObject->GetNumberField(TEXT("Yaw"));
		float Roll = jsonObject->GetNumberField(TEXT("Roll"));
		float Scale = jsonObject->GetNumberField(TEXT("Scale"));
		int32 Type = jsonObject->GetIntegerField(TEXT("Type"));

		// type 해당되는 모양을 만들자
		CreateShape(
			Type,
			FVector(PosX, PosY, PosZ),
			FRotator(Pitch, Yaw, Roll),
			FVector(Scale));
	}
}



