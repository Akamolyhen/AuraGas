// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "File")
	static bool LoadStringFromFile(FString filePath, FString& resultString);
	UFUNCTION(BlueprintCallable, Category = "File")
	static bool WriteStringToFile(TArray<FString> saveFile, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "File")
	static FString GetFilePath(FString path);
	UFUNCTION(BlueprintCallable, Category = "File")
	static FString GetFileName(FString InPath,bool bRemovePath);
	UFUNCTION(BlueprintCallable, Category = "File")
	static FString GetFileExtension(FString InPath, bool bInCludeDot);
	UFUNCTION(BlueprintCallable, Category = "File")
	static void CreateFolder(FString FolderName);
	UFUNCTION(BlueprintCallable, Category = "File")
	static void DeleteFolder(FString FolderName);
	UFUNCTION(BlueprintCallable, Category = "File")
	static bool MoveFileTo(FString To, FString From);
	UFUNCTION(BlueprintCallable, Category = "File")
	static TArray<FString> FindFileFolder(FString Path, FString Filter, bool Files, bool Directory);
};

