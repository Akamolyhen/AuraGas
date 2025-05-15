// Publish Name:HaoJunDeveloper
// Intended Year of Published:2024 
#pragma once

#include "CoreMinimal.h"
#include "ClientLatent.h"
#include "Containers/Queue.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ThirdParty/MysqlConnectorCpp/DatabaseManager.h"
#include "Experimental/Async/ConditionVariable.h"
#include "MySqlConnectionSubSystem.generated.h"

USTRUCT(BlueprintType)
struct FRowData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, Category = "Reult Row Value")
	TMap<FString, FString> Value;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnect);

class UDatabaseManager;

UCLASS(BlueprintType, Blueprintable)
class MYSQLCONNECTORTOOLS_API UMySqlConnectionSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	void UseAsync(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, bool IsSuccess);

protected:
	/**潜在操作帮助程序 */
	FClientLatent<bool>* ContinueAction;
	UPROPERTY(BlueprintAssignable, Category = "GameChat|Event")
	FDisconnect OnDisconnect;
	TQueue<DatabaseManager*, EQueueMode::Mpsc> connections;
	TSharedPtr<FCriticalSection> Mutex ;

	UE::FConditionVariable Condition;
	UE::FConditionVariable ProduceCondition;
	bool ProduceConnectionTaskBool = true;
	bool ScannerConnectionTaskBool = true;

	FString Host;
	int Port;
	FString UserName;
	FString Password;
	FString DbName;
	int MaxConnection;
	int InitSize;
	int MaxIdleTime;
	int ConnectionTimeOut;
	int ConnectionCnt = 0;

public:
	FCriticalSection IsSuccessMutex;
	bool LocalIsSuccess ;
	UFUNCTION(BlueprintCallable, Category = "Mysql|Sync")
	void Init(const FString& InHost, int InPort, const FString& InUserName, const FString& InPassword,
	          const FString& InDbName, int InMaxConnection, int InInitSize, int InMaxIdleTime, int InConnectionTimeOut);

	TSharedPtr<DatabaseManager> getConnection();

	void releaseConnection(DatabaseManager* connection);

	UFUNCTION(BlueprintCallable, Category = "Mysql|Sync")
	void Close();

	UFUNCTION(BlueprintCallable, Category = "Mysql|Sync")
	bool Connect(FString& Msg);
	

	UFUNCTION(BlueprintCallable, Category = "Mysql|Sync")
	void ProduceConnectionTask();

	UFUNCTION(BlueprintCallable, Category = "Mysql|Sync")
	void ScannerConnectionTask();

	/** execute SQL 命令（仅返回成功或失败) */
	UFUNCTION(BlueprintCallable, Category = "Mysql|Async",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"
		))
	void AExecute(TArray<FString> SqlArray, bool& IsSuccess, FString& Msg, UObject* WorldContextObject,
	              struct FLatentActionInfo LatentInfo);

	/** execute SQL 命令（仅返回成功或失败) */
	UFUNCTION(BlueprintCallable, Category = "Mysql|Async",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"
		))
	void AExecuteUpdate(TArray<FString> SqlArray, bool& IsSuccess, FString& Msg, UObject* WorldContextObject,
	                    struct FLatentActionInfo LatentInfo);

	/** 插入数据，如果录入成功，则返回行数，如果失败，则返回 0*/
	UFUNCTION(BlueprintCallable, Category = "Mysql|Async",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"
		))
	void AInsert(FString Sql, bool IsIndex, bool& IsSuccess, FString& Msg, int& index, UObject* WorldContextObject,
	             struct FLatentActionInfo LatentInfo);

	/** 更新数据 */
	UFUNCTION(BlueprintCallable, Category = "Mysql|Async",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"
		))
	void AUpdate(FString Sql, bool& IsSuccess, FString& Msg, UObject* WorldContextObject,
	             struct FLatentActionInfo LatentInfo);

	/** 从数据库中删除数据 */
	UFUNCTION(BlueprintCallable, Category = "Mysql|Async",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"
		))
	void ADelete(FString Sql, bool& IsSuccess, FString& Msg, UObject* WorldContextObject,
	             struct FLatentActionInfo LatentInfo);

	/** 查询数据*/
	UFUNCTION(BlueprintCallable, Category = "Mysql|Async",
		meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"
		))
	void AQuery(FString Sql, bool& IsSuccess, FString& Msg, TArray<FRowData>& Results, UObject* WorldContextObject,
	            struct FLatentActionInfo LatentInfo);
	
};
