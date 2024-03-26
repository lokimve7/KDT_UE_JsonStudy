// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonPawn.h"

#include "JsonUtility.h"
#include "JsonObjectConverter.h"
#include <Interfaces/IHttpRequest.h>
#include <Interfaces/IHttpResponse.h>
#include <HttpModule.h>

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

	JsonToDataExample();

	RequestShortTermForecast();
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
	info.randNum = {100, 90, 50, 77, 44, 66};

	// FJsonObjectConverter 을 활용해서 구조체 -> Json string 변경
	FString jsonString;
	FJsonObjectConverter::UStructToJsonObjectString(
		FUserInfo::StaticStruct(),
		&info,
		jsonString);

	UE_LOG(LogTemp, Warning, TEXT("json string : %s"), *jsonString);

	// Data ----> JsonObject ----> JsonString
	// 
	// JsonObject 만들자
	// FJsonObject* jsonObject = new FJsonObject();
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

	// FJsonObjectConverter 을 이용해서 JsonString 을 구조체로 변경
	FUserInfo info;
	FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &info);

	// JsonString ----> JsonObject ----> Data
	//// FString 을 JsonObject  로 만들자
	//TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	//TSharedPtr<FJsonObject> jsonObject;
	//FJsonSerializer::Deserialize(jsonReader, jsonObject);

	//// JsonObject 을 FUserInfo 로 만들자
	//FUserInfo info;
	//info.name = jsonObject->GetStringField(TEXT("name"));
	//info.age = jsonObject->GetNumberField(TEXT("age"));
	//info.height = jsonObject->GetNumberField(TEXT("height"));
	//info.gender = jsonObject->GetBoolField(TEXT("gender"));

	//// jsonArray 를 TArray 로 변환
	//TArray<TSharedPtr<FJsonValue>> jsonArray = jsonObject->GetArrayField(TEXT("interests"));
	//for (int32 i = 0; i < jsonArray.Num(); i++)
	//{
	//	info.interests.Add(jsonArray[i]->AsString());
	//	UE_LOG(LogTemp, Warning, TEXT("interest : %s"), *info.interests[i]);
	//}


	UE_LOG(LogTemp, Warning, TEXT("name - %s, age - %d, height - %f, gender - %d"),
			*info.name, info.age, info.height, info.gender);

}

void AJsonPawn::RequestShortTermForecast()
{
	// Request 생성 (HTTP 통신을 하게 해주는 객체)
	TSharedPtr<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
	// 요청해야하는 URL 셋팅
	httpRequest->SetURL(TEXT("https://jsonplaceholder.typicode.com/posts/1"));
	// 헤더를 셋팅를 추가해달라는 요청이 있으면
	httpRequest->SetHeader(TEXT("Contnet-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	// GET, POST, PUT, DELETE
	httpRequest->SetVerb(TEXT("GET"));
	// 단 POST, PUT 일 경우에는 
	//httpRequest->SetContentAsString(Json 형태의 Text 가 들어가면 된다.);
	
	// 응답이 올 때 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindUObject(this, &AJsonPawn::OnCompleteRequest);

	// 요청을 보내자
	httpRequest->ProcessRequest();
}


void AJsonPawn::OnCompleteRequest(TSharedPtr<class IHttpRequest> Request, TSharedPtr<class IHttpResponse> Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		FString jsonString = Response->GetContentAsString();

		FPostInfo postInfo;
		FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &postInfo);

		// jsonString -> jsonObject 바꾸자
		TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
		TSharedPtr<FJsonObject> jsonObject;
		FJsonSerializer::Deserialize(jsonReader, jsonObject);

		// jsonObject 를 각 Data Type 에 셋팅
		int32 userId = jsonObject->GetNumberField(TEXT("userId"));
		int32 id = jsonObject->GetNumberField(TEXT("id"));
		FString titie = jsonObject->GetStringField(TEXT("title"));
		FString body = jsonObject->GetStringField(TEXT("body"));


		/*FResponse info;
		FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &info);*/
		
		/*auto jsonObject = UJsonUtility::StringToJson(jsonString);
		auto responseObject = jsonObject->GetObjectField(TEXT("response"));
		auto bodyObject = responseObject->GetObjectField(TEXT("body"));
		auto itemsObject = bodyObject->GetObjectField(TEXT("items"));
		auto itemArray = itemsObject->GetArrayField(TEXT("item"));

		TArray<FForecastInfo> forecatInfo;
		FJsonObjectConverter::JsonArrayToUStruct(itemArray, &forecatInfo);*/

		//UE_LOG(LogTemp, Warning, TEXT("item count : %d"), itemArray.Num());

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("http result Fail"));
	}
}

