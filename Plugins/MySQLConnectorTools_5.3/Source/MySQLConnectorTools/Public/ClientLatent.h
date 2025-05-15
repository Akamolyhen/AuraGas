// Publish Name:HaoJunDeveloper
// Intended Year of Published:2024 
#pragma once

#include "CoreMinimal.h"
#include "Engine/LatentActionManager.h"
#include "LatentActions.h"

/**
 * FClientLatent创建潜在动作。
 */


template <class T>
class MYSQLCONNECTORTOOLS_API FClientLatent : public FPendingLatentAction
{
public:
    // 使用给定值调用潜在操作。
    virtual void Call(const T& Value)
    {
        Result = Value;
        Called = true;
    }

    //运算符重载，以允许像函数一样调用对象。
    void operator()(const T& Value)
    {
        Call(Value);
    }

    // 取消潜在操作。
    void Cancel()
    {
       
    }

    // 用于创建新潜在操作的构造函数。
    FClientLatent(FWeakObjectPtr RequestObj, T& ResultParam, const FLatentActionInfo& LatentInfo)
        : Called(false)
        , Request(RequestObj)
        , ExecutionFunction(LatentInfo.ExecutionFunction)
        , OutputLink(LatentInfo.Linkage)
        , CallbackTarget(LatentInfo.CallbackTarget)
        , Result(ResultParam)
    {
    }

    //更新潜在操作的操作。
    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(Called, ExecutionFunction, OutputLink, CallbackTarget);
       
    }
    // 通知对象已被销毁。
    virtual void NotifyObjectDestroyed() override
    {
        Cancel();
    }
    //通知操作已中止。
    virtual void NotifyActionAborted() override
    {
        Cancel();
    }

private:
    bool Called; // 指示是否已调用操作。
    FWeakObjectPtr Request; // 指向请求对象的弱指针。
public:
    const FName ExecutionFunction; // 要执行的函数的名称。
    const int32 OutputLink; //链接到链中的下一个操作。
    const FWeakObjectPtr CallbackTarget; //指向回调目标的弱指针。
    T& Result; // 对结果变量的引用。
};