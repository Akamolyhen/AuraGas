// Publish Name:HaoJunDeveloper
// Intended Year of Published:2024 
#pragma once

#include "CoreMinimal.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

/**
 *FWorkerThread封装了可以定期执行工作的线程。
 */
class FWorkerThread : public FRunnable
{
public:
	FWorkerThread(TFunction<bool()> InWork, float WaitSeconds = 0.001f);
	FWorkerThread(TFunction<bool()> InWork, TFunction<void()> InEnd, float WaitSeconds = 0.001f);
	virtual ~FWorkerThread();
	
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	TFunction<bool()> Work; // 完成工作的函数。
	TFunction<void()> End; // 线程结束时调用的函数。
	float Seconds; // 每次工作执行之间的间隔。
	FThreadSafeBool ContinueRun; // 控制线程的运行状态。
	TUniquePtr<FRunnableThread>  Thread;
};