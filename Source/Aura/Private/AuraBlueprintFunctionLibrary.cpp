// Fill out your copyright notice in the Description page of Project Settings.
 
 
#include "AuraBlueprintFunctionLibrary.h"
#include "DesktopPlatformModule.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
 
bool UAuraBlueprintFunctionLibrary::LoadStringFromFile(FString filePath, FString& resultString)
{
	if (!filePath.IsEmpty())
	{
		if (FFileHelper::LoadFileToString(resultString, *filePath))
		{
			return true;
		}
		else
		{
			//error
		}
	}
	return false;
}
 
bool UAuraBlueprintFunctionLibrary::WriteStringToFile(TArray<FString> saveFile, FString filePath)
{
	if (!filePath.IsEmpty())
	{
		if (FFileHelper::SaveStringArrayToFile(saveFile, *filePath))
		{
			return true;
		}
		else
		{
			//error
		}
	}
	return false;
}
 
FString UAuraBlueprintFunctionLibrary::GetFilePath(FString path)
{
	FString result;
	result = FPaths::GetPath(*path);
	return result;
}
 
FString UAuraBlueprintFunctionLibrary::GetFileName(FString InPath, bool bRemovePath)
{
	return FPaths::GetBaseFilename(*InPath,bRemovePath);
}
 
FString UAuraBlueprintFunctionLibrary::GetFileExtension(FString InPath, bool bInCludeDot)
{
	return FPaths::GetExtension(*InPath,bInCludeDot);
}
 
void UAuraBlueprintFunctionLibrary::CreateFolder(FString FolderName)
{
	//FString path = FPaths::ProjectContentDir();
	FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*FolderName);
}
 
void UAuraBlueprintFunctionLibrary::DeleteFolder(FString FolderName)
{
	//FString path = FPaths::ProjectContentDir();
	FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*FolderName);
}
 
bool UAuraBlueprintFunctionLibrary::MoveFileTo(FString To, FString From)
{
	return IFileManager::Get().Move(*To,*From);
}
 
TArray<FString> UAuraBlueprintFunctionLibrary::FindFileFolder(FString Path, FString Filter, bool Files, bool Directory)
{
	TArray<FString> FilePathList;
	FilePathList.Empty();
	FFileManagerGeneric::Get().FindFilesRecursive(FilePathList, *Path, *Filter, Files, Directory);
	return FilePathList;
}