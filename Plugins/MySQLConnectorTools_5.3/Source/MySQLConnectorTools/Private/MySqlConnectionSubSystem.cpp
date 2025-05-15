// Publish Name:HaoJunDeveloper
// Intended Year of Published:2024 
#define STATIC_CONCPP

#include "..\Public\MySqlConnectionSubSystem.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Async/AsyncWork.h"
#include "WorkerThread.h"
#include "ThirdParty/MysqlConnectorCpp/DatabaseManager.h"

void UMySqlConnectionSubSystem::UseAsync(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, bool IsSuccess)
{
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		FClientLatent<bool>* Kont = LatentActionManager.FindExistingAction<FClientLatent<bool>>(
			LatentInfo.CallbackTarget, LatentInfo.UUID);

		if (Kont != nullptr)
		{
			Kont->Cancel();
			LatentActionManager.RemoveActionsForObject(LatentInfo.CallbackTarget);
		}
		UE_LOG(LogTemp, Warning, TEXT("UseAsync"));
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
		                                 ContinueAction = new FClientLatent<bool>(this, IsSuccess, LatentInfo));
	}
}


void UMySqlConnectionSubSystem::Init(const FString& InHost, int InPort, const FString& InUserName,
                                  const FString& InPassword, const FString& InDbName, int InMaxConnection,
                                  int InInitSize, int InMaxIdleTime,
                                  int InConnectionTimeOut)
{
	Host = InHost;
	Port = InPort;
	UserName = InUserName;
	Password = InPassword;
	DbName = InDbName;
	MaxConnection = InMaxConnection;
	InitSize = InInitSize;
	MaxIdleTime = InMaxIdleTime;
	ConnectionTimeOut = InConnectionTimeOut;
	Mutex = MakeShareable(new FCriticalSection());

}

TSharedPtr<DatabaseManager> UMySqlConnectionSubSystem::getConnection()
{
	FScopeLock Lock(Mutex.Get());
	bool IsSuccess = false;
	while (connections.IsEmpty())
	{
		Condition.Wait(*Mutex);
		FPlatformProcess::Sleep(0.001f);
	}
	DatabaseManager* connection;
	connections.Dequeue(connection);
	// 创建一个自定义删除器
	TUniqueFunction<void(DatabaseManager*)> CustomDeleter = [this](DatabaseManager* connection)
	{
			FScopeLock Lock(Mutex.Get());

		connection->SetIdleStartTime();
		connections.Enqueue(connection);
		connection = nullptr;
		Condition.NotifyOne();
	};
	TSharedPtr<DatabaseManager> sp(connection, MoveTemp(CustomDeleter));
	//TSharedPtr<DatabaseManager> ManagedPointer = TSharedPtr<DatabaseManager>(connection, CustomDeleter);
	ProduceCondition.NotifyOne();
	return sp;
}

void UMySqlConnectionSubSystem::releaseConnection(DatabaseManager* connection)
{
	FScopeLock Lock(Mutex.Get());// 使用FScopeLock来自动锁定和解锁FCriticalSection
	connection->SetIdleStartTime();
	connections.Enqueue(connection);
	Condition.NotifyOne(); // 使用FConditionVariable的NotifyOne方法
}

void UMySqlConnectionSubSystem::Close()
{
	ProduceConnectionTaskBool = false;
	ScannerConnectionTaskBool = false;
	DatabaseManager* conns;
	while (connections.Dequeue(conns))
	{
		conns->CloseConnection();
		conns = nullptr;
	}
}

bool UMySqlConnectionSubSystem::Connect(FString& Msg)
{
	bool bAllConnectionsSuccessful = true;
	FString LocalHost = Host;
	FString LocalUserName =UserName;
	FString LocalPassword = Password;
	FString LocalDbName = DbName;
	bool IsConns = false;
	const std::string tHost(TCHAR_TO_UTF8(*LocalHost));
	const std::string tUserName(TCHAR_TO_UTF8(*LocalUserName));
	const std::string tPassword(TCHAR_TO_UTF8(*LocalPassword));
	const std::string tDbName(TCHAR_TO_UTF8(*LocalDbName));
	for (int i = 0; i < InitSize; ++i)
	{
	
		DatabaseManager* conns = CreateDatabaseManager(tHost, Port, tUserName, tPassword, tDbName, i);
		conns->SetIdleStartTime();
		IsConns = conns->Connect();
		if (IsConns)
		{
			Msg = FString::Printf(TEXT("The Database Connection Is Successful%d: %s"), i, *Host);
			connections.Enqueue(conns);
			ConnectionCnt++;
		}
		else
		{
			Msg = FString::Printf(TEXT("The Database Connection Failed%d: %s"), i, *Host);
			bAllConnectionsSuccessful = false;
			break;
		}
		UE_LOG(LogTemp, Warning, TEXT("Database status：%s"), *Msg);
	}

	//处理任何必要的清
	if (!bAllConnectionsSuccessful)
	{
		Close();
	}
	ProduceConnectionTask();
	ScannerConnectionTask();
	return bAllConnectionsSuccessful;
}


void UMySqlConnectionSubSystem::ProduceConnectionTask()
{


	TSharedPtr<UMySqlConnectionSubSystem> SharedCopyThis = MakeShareable(this);
	FRunnableThread::Create(new FWorkerThread([SharedThis= SharedCopyThis ]
	{
			if (SharedThis.IsValid())
			{
				// Check if Mutex is valid
				if (!SharedThis->Mutex.IsValid())
				{
					UE_LOG(LogTemp, Error, TEXT("Mutex is not initialized!"));
					return false;
				}
				
				FScopeLock Lock(SharedThis->Mutex.Get());

				bool IsSuccess = false;
				while (!SharedThis->connections.IsEmpty())
				{
					// Ensure Mutex is still valid before waiting
					if (!SharedThis->Mutex.IsValid())
					{
						UE_LOG(LogTemp, Error, TEXT("Mutex became invalid during wait!"));
						return false;
					}
					
					SharedThis->ProduceCondition.Wait(*SharedThis->Mutex);
					FPlatformProcess::Sleep(0.01f);
				}

				if (SharedThis->ConnectionCnt <2)
				{
					// 创建本地副本，确保线程安全
					FString LocalHost = SharedThis->Host;
					FString LocalUserName = SharedThis->UserName;
					FString LocalPassword = SharedThis->Password;
					FString LocalDbName = SharedThis->DbName;

					if (LocalHost.IsEmpty() || LocalUserName.IsEmpty() || LocalPassword.IsEmpty() || LocalDbName.IsEmpty())
					{
						UE_LOG(LogTemp, Error, TEXT("Database credentials are invalid."));
						return false;
					}

					const std::string tHost(TCHAR_TO_UTF8(*LocalHost));
					const std::string tUserName(TCHAR_TO_UTF8(*LocalUserName));
					const std::string tPassword(TCHAR_TO_UTF8(*LocalPassword));
					const std::string tDbName(TCHAR_TO_UTF8(*LocalDbName));

					FString Msg;
					DatabaseManager* conns = CreateDatabaseManager(tHost, SharedThis->Port, tUserName, tPassword, tDbName, 0);
					conns->SetIdleStartTime();
					std::string _msg;
					IsSuccess = conns->Connect();
					if (IsSuccess)
					{
						Msg = FString::Printf(TEXT("The database connection is successful: %s"), *LocalHost);
					}
					else
					{
						Msg = FString::Printf(TEXT("Failed to add database connection: %s"), *LocalHost);
					}

					UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
					SharedThis->connections.Enqueue(conns);
					SharedThis->ConnectionCnt++;
				}
				SharedThis->Condition.NotifyOne();
				
			}
			return SharedThis->ProduceConnectionTaskBool;
	}), TEXT("ProduceConnectionTask"));
}

void UMySqlConnectionSubSystem::ScannerConnectionTask()
{
	TSharedPtr<UMySqlConnectionSubSystem> SharedCopyThis = MakeShareable(this);
	FRunnableThread::Create(new FWorkerThread([SharedThis = SharedCopyThis]
	{
		FPlatformProcess::Sleep(SharedThis->MaxIdleTime);

		if (SharedThis->ScannerConnectionTaskBool)
		{
			while (SharedThis->ConnectionCnt > SharedThis->InitSize && SharedThis->ScannerConnectionTaskBool)
			{
				// Check if Mutex is valid
				if (!SharedThis->Mutex.IsValid())
				{
					UE_LOG(LogTemp, Error, TEXT("Mutex is not initialized!"));
					return false;
				}

				UE_LOG(LogTemp, Log, TEXT("Mutex address: %p"), &SharedThis->Mutex);

				FScopeLock Lock(SharedThis->Mutex.Get());

				DatabaseManager* conns;
				if (SharedThis->connections.Dequeue(conns))
				{
					if (conns->GetConnectionAliveTime() < std::chrono::seconds(SharedThis->MaxIdleTime))
					{
						conns->CloseConnection();
						conns = nullptr;
						SharedThis->ConnectionCnt--;
						UE_LOG(LogTemp, Warning, TEXT("Closed an idle connection. Remaining connections: %d"), SharedThis->ConnectionCnt);
					}
					else
					{
						SharedThis->connections.Enqueue(conns);
						break;
					}
				}

				FPlatformProcess::Sleep(FMath::Max(30.0f, SharedThis->MaxIdleTime / 2.0f));
			}
		}

		return SharedThis->ScannerConnectionTaskBool;
	}), TEXT("ScannerConnectionTask"));
}

void UMySqlConnectionSubSystem::AExecute(TArray<FString> SqlArray, bool& IsSuccess, FString& Msg,
                                      UObject* WorldContextObject, struct FLatentActionInfo LatentInfo)
{
	UseAsync(WorldContextObject, LatentInfo, IsSuccess);

	FRunnableThread::Create(new FWorkerThread([this, SqlArray, &IsSuccess, &Msg]
	{
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			OnDisconnect.Broadcast();
		});

		TSharedPtr<DatabaseManager> conns = getConnection();
		std::vector<std::string> Std_SqlArray;
		Std_SqlArray.reserve(SqlArray.Num());
		for (const FString& UE_Str : SqlArray)
		{
			// 将FString转换为std::string
			std::string Std_Str(TCHAR_TO_UTF8(*UE_Str));
			Std_SqlArray.push_back(Std_Str);
		}
		IsSuccess = conns->ExecuteData(Std_SqlArray);
		if (IsSuccess)
		{
			Msg = TEXT("ExecuteData Success");
		}
		else
		{
			Msg = TEXT("ExecuteData Failed");
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		if (ContinueAction)
		{
			ContinueAction->Call(IsSuccess);
			ContinueAction = nullptr;
		}

		return false;
	}), TEXT("MysqlConnect Execute PollingThread"));
}

void UMySqlConnectionSubSystem::AExecuteUpdate(TArray<FString> SqlArray, bool& IsSuccess, FString& Msg,
                                            UObject* WorldContextObject, FLatentActionInfo LatentInfo)
{
	UseAsync(WorldContextObject, LatentInfo, IsSuccess);
	TSharedPtr<DatabaseManager> conns = getConnection();
	FRunnableThread::Create(new FWorkerThread([this, SqlArray, &IsSuccess, &Msg, &conns]
	{
		AsyncTask(ENamedThreads::Type::GameThread, [this]
		{
			OnDisconnect.Broadcast();
		});


		std::vector<std::string> Std_SqlArray;
		Std_SqlArray.reserve(SqlArray.Num());
		for (const FString& UE_Str : SqlArray)
		{
			// 将FString转换为std::string
			std::string Std_Str(TCHAR_TO_UTF8(*UE_Str));
			Std_SqlArray.push_back(Std_Str);
		}
		if (conns)
		{
			// sp 不为空，可以安全地使用
			IsSuccess = conns->ExecuteUpdateData(Std_SqlArray);
		}
		else
		{
			// sp 为空，不能解引用或使用
			UE_LOG(LogTemp, Warning, TEXT("conns is nullptr"));
		}

		if (IsSuccess)
		{
			Msg = TEXT("AExecuteUpdateData Success");
		}
		else
		{
			Msg = TEXT("AExecuteUpdateData Failed");
		}
		UE_LOG(LogTemp, Warning, TEXT("%d: %s"), IsSuccess, *Msg);
		//releaseConnection(conns);

		if (ContinueAction)
		{
			{
				FScopeLock Lock(&IsSuccessMutex);
				LocalIsSuccess = IsSuccess;
			}
			ContinueAction->Call(IsSuccess);
			ContinueAction = nullptr;
		}

		return false;
	}), TEXT("MysqlConnect ExecuteUpdate PollingThread"));
}


void UMySqlConnectionSubSystem::AInsert(FString Sql, bool IsIndex, bool& IsSuccess, FString& Msg, int& index,
                                     UObject* WorldContextObject, struct FLatentActionInfo LatentInfo)
{
	UseAsync(WorldContextObject, LatentInfo, IsSuccess);

	FRunnableThread::Create(new FWorkerThread([this, Sql, IsIndex, &IsSuccess, &Msg, &index]
	{
		AsyncTask(ENamedThreads::Type::GameThread, [this]
		{
			OnDisconnect.Broadcast();
		});
		FString SqlCopy = Sql;
		TSharedPtr<DatabaseManager> conns = getConnection();
		IsSuccess = conns->InsertData(TCHAR_TO_UTF8(*Sql), index);
		if (IsSuccess)
		{
			Msg = TEXT("InsertData Success");
		}
		else
		{
			Msg = TEXT("InsertData Failed");
		}
		UE_LOG(LogTemp, Warning, TEXT("%s：ExecuteData;%s"), *Msg, *SqlCopy);
		//releaseConnection(conns);
		// Finish the latent action
		if (ContinueAction)
		{
			ContinueAction->Call(IsSuccess);
			ContinueAction = nullptr;
		}


		return false;
	}), TEXT("MysqlConnect Insert PollingThread"));
}

void UMySqlConnectionSubSystem::AUpdate(FString Sql, bool& IsSuccess, FString& Msg, UObject* WorldContextObject,
                                     struct FLatentActionInfo LatentInfo)
{
	UseAsync(WorldContextObject, LatentInfo, IsSuccess);

	FRunnableThread::Create(new FWorkerThread([this, Sql, &IsSuccess, &Msg]
	{
		AsyncTask(ENamedThreads::Type::GameThread, [this]
		{
			OnDisconnect.Broadcast();
		});
		TSharedPtr<DatabaseManager> conns = getConnection();
		FString SqlCopy = Sql;
		IsSuccess = conns->UpdateData(TCHAR_TO_UTF8(*Sql));
		if (IsSuccess)
		{
			Msg = TEXT("UpdateData Success");
		}
		else
		{
			Msg = TEXT("UpdateData Failed");
		}
		UE_LOG(LogTemp, Warning, TEXT("%s: Execute the statement：%s"), *Msg, *SqlCopy);
		//releaseConnection(conns);

		if (ContinueAction)
		{
			ContinueAction->Call(IsSuccess);
			ContinueAction = nullptr;
		}

		return false;
	}), TEXT("MysqlConnect Update PollingThread"));
}

void UMySqlConnectionSubSystem::ADelete(FString Sql, bool& IsSuccess, FString& Msg, UObject* WorldContextObject,
                                     struct FLatentActionInfo LatentInfo)
{
	UseAsync(WorldContextObject, LatentInfo, IsSuccess);

	FRunnableThread::Create(new FWorkerThread([this, Sql, &IsSuccess, &Msg]
	{
		AsyncTask(ENamedThreads::Type::GameThread, [this]
		{
			OnDisconnect.Broadcast();
		});
		TSharedPtr<DatabaseManager> conns = getConnection();
		FString SqlCopy = Sql;
		IsSuccess = conns->DeleteData(TCHAR_TO_UTF8(*Sql));
		if (IsSuccess)
		{
			Msg = TEXT("DeleteData Success");
		}
		else
		{
			Msg = TEXT("DeleteData Failed");
		}
		UE_LOG(LogTemp, Warning, TEXT("%s：Execute the statement：%s"), *Msg, *SqlCopy);
		//releaseConnection(conns);

		if (ContinueAction)
		{
			ContinueAction->Call(IsSuccess);
			ContinueAction = nullptr;
		}

		return false;
	}), TEXT("MysqlConnect Delete PollingThread"));
}

void UMySqlConnectionSubSystem::AQuery(FString Sql, bool& IsSuccess, FString& Msg, TArray<FRowData>& Results,
                                    UObject* WorldContextObject, struct FLatentActionInfo LatentInfo)
{
	UseAsync(WorldContextObject, LatentInfo, IsSuccess);

	FRunnableThread::Create(new FWorkerThread([this, SqlStr = Sql, &IsSuccess, &Msg, &Results]
	{
		AsyncTask(ENamedThreads::Type::GameThread, [this]
		{
			OnDisconnect.Broadcast();
		});
		TSharedPtr<DatabaseManager> conns = getConnection();
		std::vector<std::map<std::string, std::string>> arr;
		IsSuccess = conns->QueryData(TCHAR_TO_UTF8(*SqlStr), arr);
		UE_LOG(LogTemp, Warning, TEXT("Query: %s"), *Msg);
		if (Results.IsEmpty())
		{
			for (auto iter : arr)
			{
				FRowData _FRow;
				std::map<std::string, std::string>::iterator val;
				for (val = iter.begin(); val != iter.end(); ++val) //������
				{
					_FRow.Value.Add(val->first.c_str(), UTF8_TO_TCHAR(val->second.c_str()));
				}

				Results.Add(_FRow);
			}
		}
		if (IsSuccess)
		{
			Msg = TEXT("QueryData Success");
		}
		else
		{
			Msg = TEXT("QueryData Failed");
		}
		UE_LOG(LogTemp, Warning, TEXT("%s: Execute the statement：%s"), *Msg, *SqlStr);

		//releaseConnection(conns);

		if (ContinueAction)
		{
			ContinueAction->Call(IsSuccess);
			ContinueAction = nullptr;
		}
		return false;
	}), TEXT("MysqlConnect Query PollingThread"));
}
