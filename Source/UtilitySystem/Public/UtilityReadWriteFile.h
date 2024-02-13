// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityReadWriteFile.generated.h"

/**
 * 
 */
UCLASS()
class UTILITYSYSTEM_API UUtilityReadWriteFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 *
	 * @param filePath	The path of the file
	 * @param bOutSuccess	If the action was a success or not
	 * @param outInfoMessage	More information about the action's result
	 *
	 * @return the string content of the file
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility|ReadWriteFile")
	static FString ReadStringFromFile(FString filePath, bool& bOutSuccess, FString& outInfoMessage);

	 /**
	 * Create a text file with the content
	 *
	 * @param filePath	The path of the file
	 * @param string	The string to want to write in the file
	 * @param bOutSuccess	If the action was a success or not
	 * @param outInfoMessage	More information about the action's result
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility|ReadWriteFile")
	static void WriteStringToFile(FString filePath, FString string, bool& bOutSuccess, FString& outInfoMessage);
};
