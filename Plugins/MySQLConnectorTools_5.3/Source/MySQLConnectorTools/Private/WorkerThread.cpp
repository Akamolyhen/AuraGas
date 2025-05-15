// Publish Name:HaoJunDeveloper
// Intended Year of Published:2024 
#include "WorkerThread.h"
#include "HAL/PlatformProcess.h"

FWorkerThread::FWorkerThread(TFunction<bool()> InWork, float WaitSeconds)
	: Work(InWork)
	, End( {})
	, Seconds(WaitSeconds)
	, ContinueRun(true)
{
	
}

FWorkerThread::FWorkerThread(TFunction<bool()> InWork, TFunction<void()> InEnd, float WaitSeconds)
	: Work(InWork)
	, End(InEnd)
	, Seconds(WaitSeconds)
	, ContinueRun(true)
{
	
}

FWorkerThread::~FWorkerThread()
{
	//确保线程已正确停止并释放资源。
	if (Thread)
	{
		Stop();
		Thread->WaitForCompletion();
		Thread.Reset();
	}
}

uint32 FWorkerThread::Run()
{
	while (ContinueRun)
	{
		if (!Work())
		{
			ContinueRun = false;
		}

		FPlatformProcess::Sleep(Seconds);
	}

	return 0;
}

void FWorkerThread::Stop()
{
	ContinueRun = false;
}

void FWorkerThread::Exit()
{
	Stop();
	if (End)
	{
		End();
	}
}