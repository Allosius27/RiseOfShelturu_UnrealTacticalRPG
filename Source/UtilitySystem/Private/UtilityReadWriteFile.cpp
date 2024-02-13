// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityReadWriteFile.h"

#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"

FString UUtilityReadWriteFile::ReadStringFromFile(FString filePath, bool& bOutSuccess, FString& outInfoMessage)
{
	// Check if the file exists
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath))
	{
		bOutSuccess = false;
		outInfoMessage = FString::Printf(TEXT("Read String From File Failed - File doesn't exist - '%s'"), *filePath);
		return "";
	}

	FString RetString = "";

	// Try to read the file. Output goes in RetString
	if(!FFileHelper::LoadFileToString(RetString, *filePath))
	{
		bOutSuccess = false;
		outInfoMessage = FString::Printf(TEXT("Read String From File Failed - Was not able to read file. Is this a text file ? - '%s'"), *filePath);
		return "";
	}

	bOutSuccess = true;
	outInfoMessage = FString::Printf(TEXT("Read String From File Succeeded - '%s'"), *filePath);
	return RetString;
}

void UUtilityReadWriteFile::WriteStringToFile(FString filePath, FString string, bool& bOutSuccess,
	FString& outInfoMessage)
{
	// Try to write the string into the file
	if(!FFileHelper::SaveStringToFile(string, *filePath))
	{
		bOutSuccess = false;
		outInfoMessage = FString::Printf(TEXT("Write String To File Failed - Was not able to write to file. Is your file read only ? Is the path valid ? - '%s'"), *filePath);
		return;
	}

	bOutSuccess = true;
	outInfoMessage = FString::Printf(TEXT("Write String To File Succeeded - '%s'"), *filePath);
}
