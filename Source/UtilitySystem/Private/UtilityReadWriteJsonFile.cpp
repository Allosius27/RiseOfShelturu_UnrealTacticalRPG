// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityReadWriteJsonFile.h"
#include "UtilityReadWriteFile.h"

#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

FUtilityJsonTestStruct UUtilityReadWriteJsonFile::ReadStructFromJsonFile(FString jsonFilePath, bool useManualPath, bool& bOutSuccess,
	FString& outInfoMessage)
{
	// Try to read generic json object from file
	FString filePath = FPaths::ProjectSavedDir() + TEXT("SavedDatas/") + jsonFilePath;
	if(useManualPath)
	{
		filePath = jsonFilePath;
	}
	TSharedPtr<FJsonObject> jsonObject = ReadJson(filePath, bOutSuccess, outInfoMessage);
	if(!bOutSuccess)
	{
		return FUtilityJsonTestStruct();
	}

	FUtilityJsonTestStruct retUtilityJsonTestStruct;

	// Try to convert generic json object to the desired structure. Output goes in retUtilityJsonTestStruct
	if(!FJsonObjectConverter::JsonObjectToUStruct<FUtilityJsonTestStruct>(jsonObject.ToSharedRef(), &retUtilityJsonTestStruct))
	{
		bOutSuccess = false;
		outInfoMessage = FString::Printf(TEXT("Read Struct Json Failed - Was not able to convert the json object to your desired structure. Is it the right format / struct ? - '%s'"), *jsonFilePath);
		return FUtilityJsonTestStruct();
	}

	bOutSuccess = true;
	outInfoMessage = FString::Printf(TEXT("Read Struct Json Succeeded - '%s'"), *jsonFilePath);
	return retUtilityJsonTestStruct;
}

void UUtilityReadWriteJsonFile::WriteStructToJsonFile(FString jsonFilePath, bool useManualPath, FUtilityJsonTestStruct utilityStruct,
	bool& bOutSuccess, FString& outInfoMessage)
{
	// Try to convert struct to generic json object
	TSharedPtr<FJsonObject> jsonObject = FJsonObjectConverter::UStructToJsonObject(utilityStruct);
	if(jsonObject == nullptr)
	{
		bOutSuccess = false;
		outInfoMessage = FString::Printf(TEXT("Write Struct Json Failed - Was not able to convert the struct to a json object. This shouldn't really happen."));
		return;
	}

	// Try to write json to file
	
	FDateTime CurrentDateTime = FDateTime::Now();
	UE_LOG(LogTemp, Log, TEXT("%s"), *CurrentDateTime.ToString());
	FText TimeStamp = FText::AsDateTime(CurrentDateTime, EDateTimeStyle::Short, EDateTimeStyle::Medium);
	UE_LOG(LogTemp, Log, TEXT("%s"), *TimeStamp.ToString());
	FText DateStamp = FText::AsDate(CurrentDateTime, EDateTimeStyle::Long);
	UE_LOG(LogTemp, Log, TEXT("%s"), *DateStamp.ToString());

	FString date = "_" +FString::FromInt(CurrentDateTime.GetYear()) + "_" + FString::FromInt(CurrentDateTime.GetMonth()) + 
		"_" + FString::FromInt(CurrentDateTime.GetDay())+ "-" + FString::FromInt(CurrentDateTime.GetHour()) + 
			"_" + FString::FromInt(CurrentDateTime.GetMinute())+ "_" + FString::FromInt(CurrentDateTime.GetSecond());
	FString filePath = FPaths::ProjectSavedDir() + TEXT("SavedDatas/") + date + TEXT("_") + jsonFilePath;
	//FString filePath = FPaths::ProjectLogDir() + + TEXT("SavedDatas/") + jsonFilePath;
	//FString filePath = FPaths::ProjectPersistentDownloadDir() + TEXT("SavedDatas/") + jsonFilePath;
	if(useManualPath)
	{
		filePath = jsonFilePath;
	}
	WriteJson(filePath, jsonObject, bOutSuccess, outInfoMessage);
}

TSharedPtr<FJsonObject> UUtilityReadWriteJsonFile::ReadJson(FString jsonFilePath, bool& bOutSuccess,
                                                            FString& outInfoMessage)
{
	// Try to read File
	FString jsonString = UUtilityReadWriteFile::ReadStringFromFile(jsonFilePath, bOutSuccess, outInfoMessage);
	if(!bOutSuccess)
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> retJsonObject;

	// Try to convert string to json object. Output goes in retJsonObject
	if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(jsonString), retJsonObject))
	{
		bOutSuccess = false;
		outInfoMessage = FString::Printf(TEXT("Read Json Failed - Was not able to deserialize the json string. Is it the right format ? - '%s'"), *jsonString);
		return nullptr;
	}

	bOutSuccess = true;
	outInfoMessage = FString::Printf(TEXT("Read Json Succeeded - '%s'"), *jsonFilePath);
	return retJsonObject;
}

void UUtilityReadWriteJsonFile::WriteJson(FString jsonFilePath, TSharedPtr<FJsonObject> jsonObject, bool& bOutSuccess,
	FString& outInfoMessage)
{
	FString jsonString;

	// Try to convert json object to string. Output goes in JsonString
	if(!FJsonSerializer::Serialize(jsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&jsonString, 0)))
	{
		bOutSuccess = false;
		outInfoMessage = FString::Printf(TEXT("Write Json Failed - Was not able to serialize the json to string. Is the JsonObject valid ?"));
		return;
	}

	// Try to write json string to file
	UUtilityReadWriteFile::WriteStringToFile(jsonFilePath, jsonString, bOutSuccess, outInfoMessage);
	if(!bOutSuccess)
	{
		return;
	}

	bOutSuccess = true;
	outInfoMessage = FString::Printf(TEXT("Write Json Succeeded - '%s'"), *jsonFilePath);
}
