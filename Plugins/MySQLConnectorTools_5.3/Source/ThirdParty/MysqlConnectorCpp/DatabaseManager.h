#pragma once

#if defined(_WIN32) || defined(_WIN64)
#ifdef DATABASEMANAGER_EXPORTS
        #define DATABASEMANAGER_API __declspec(dllexport)
    #else
        #define DATABASEMANAGER_API __declspec(dllimport)
    #endif
#else
#define DATABASEMANAGER_API
#endif


#include <string>
#include <chrono>
#include <vector>
#include <map>

class DatabaseManagerImpl;

class DATABASEMANAGER_API DatabaseManager
{
private:

    DatabaseManagerImpl* pImpl;

public:
    DatabaseManager(const std::string& InHost, int InPort, const std::string& InUser,
        const std::string& InPassword, const std::string& InSchemaName, int InIdex);
    ~DatabaseManager();

    bool Connect();

    void CloseConnection();

    bool ExecuteData(std::vector<std::string> SqlArray);

    bool AExecuteData(const std::string& query);

    bool ExecuteUpdateData(std::vector<std::string> SqlArray);

    bool AExecuteUpdateData(const std::string& query);

    bool InsertData(const std::string& query, int& Index);

    bool QueryData(const std::string& query, std::vector<std::map<std::string, std::string>>& arr);

    bool UpdateData(const std::string& query);

    bool DeleteData(const std::string& query);

    // 设置空闲起始时间
    void SetIdleStartTime();

    // 获取连接存活时间
    std::chrono::seconds GetConnectionAliveTime() const;

    void AddMessage(const std::string& msg);

    bool IsEmpty();
};

// 工厂函数
extern "C" DATABASEMANAGER_API DatabaseManager* CreateDatabaseManager(const std::string& InHost, int InPort, const std::string & InUser,
    const std::string & InPassword, const std::string & InSchemaName, int InIdex);
