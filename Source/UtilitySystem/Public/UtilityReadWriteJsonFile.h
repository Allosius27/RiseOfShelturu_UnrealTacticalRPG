// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityReadWriteJsonFile.generated.h"

class FJsonObject;

USTRUCT(BlueprintType, Category = "Utility|ReadWriteJson")
struct FUtilityJsonTestStruct
{
	GENERATED_BODY()

public:
	// Variables visible in both Blueprint and is in Json
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Utility|ReadWriteJson")
	FString TestString = "Name";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Utility|ReadWriteJson")
	bool TestBool = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Utility|ReadWriteJson")
	float TestFloat = 125.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Utility|ReadWriteJson")
	int TestInt = 100;

	// Variables not visible in Blueprint, but still visible in Json
	
	UPROPERTY()
	FVector TestVector = {1.0f, 0.0f, 1.0f};

	UPROPERTY()
	FRotator TestRotator = {90.0f, 0.0f, 90.0f};

	UPROPERTY()
	FTransform TestTransform;
	
	// Variables not visible in Blueprint or Json

	FString TestOtherString = "This variable will not be in the json because it's not flagged as an UPROPERTY()";
};

UCLASS()
class UTILITYSYSTEM_API UUtilityReadWriteJsonFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/**
	* Open a json file, read its content and dump it into a structure
	*
	* @param jsonFilePath	The path of the json file
	* @param bOutSuccess	If the action was a success or not
	* @param outInfoMessage	More information about the action's result
	*
	* @return the structure content of the json file
	*/
	UFUNCTION(BlueprintCallable, Category = "Utility|ReadWriteJson")
	static FUtilityJsonTestStruct ReadStructFromJsonFile(FString jsonFilePath, bool useManualPath, bool& bOutSuccess, FString& outInfoMessage);

	/**
	* Convert a structure to a string, create a json file and dump the string in it
	*
	* @param jsonFilePath	The path of the json file
	* @param utilityStruct			The structure to want to write in the file
	* @param bOutSuccess	If the action was a success or not
	* @param outInfoMessage	More information about the action's result
	*
	* @return the structure content of the json file
	*/
	UFUNCTION(BlueprintCallable, Category = "Utility|ReadWriteJson")
	static void WriteStructToJsonFile(FString jsonFilePath, bool useManualPath, FUtilityJsonTestStruct utilityStruct, bool& bOutSuccess, FString& outInfoMessage);
	
	
	/**
	 * Open a json file, read its content and convert it to a JsonObject
	 *
	 * @param jsonFilePath	The path of the json file
	 * @param bOutSuccess	If the action was a success or not
	 * @param outInfoMessage	More information about the action's result
	 *
	 * @return the JsonObject content of the json file
	 */
	static TSharedPtr<FJsonObject> ReadJson(FString jsonFilePath, bool& bOutSuccess, FString& outInfoMessage);

	/**
	 * Convert a JsonObject to a string, create a json file and dump the string in it
	 *
	 * @param jsonFilePath	The path of the json file
	 * @param jsonObject	The JsonObject to want to write in the file
	 * @param bOutSuccess	If the action was a success or not
	 * @param outInfoMessage	More information about the action's result
	 */
	static void WriteJson(FString jsonFilePath, TSharedPtr<FJsonObject> jsonObject, bool& bOutSuccess, FString& outInfoMessage);
};
