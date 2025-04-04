//
//  ScreenshotMgr.cpp
//  MultithreadingApp
//
//  Created by Khushneet Inder on 4/4/2025.
//

#include "ScreenshotMgr.hpp"
#include <thread>
#include <iostream>

ScreenshotMgr::ScreenshotMgr() 
{
	
}

ScreenshotMgr::~ScreenshotMgr()
{
	mQueueCondition.notify_all();
	if (mWorker.joinable()) {
		mWorker.join();
	}
}

void ScreenshotMgr::Take(int request, CallBack notify, void* objectToNotify)
{
	{
		std::lock_guard<std::mutex> guard(mMutex);
		mRequestQueue.emplace_back(request);
		++mShotCount;
		++mRemainingRequestCount;
		
		if (!mWorker.joinable()) {
			mWorker = std::thread(&ScreenshotMgr::Save, this, notify, objectToNotify);
		}
	}

	// Notify the condition variable in thread that there is request to process
	mQueueCondition.notify_one(); // Notify one consumer
}

void ScreenshotMgr::Save(CallBack notify, void* objectToNotify)
{
	while (true) {
		//Use unique lock since we need to unlock it
		std::unique_lock<std::mutex> lock(mMutex);
		
		// Blocks the thread by taking the lock if the there is no request
		mQueueCondition.wait(lock, [this] { return !mRequestQueue.empty(); });
		
		if (!mRequestQueue.empty()) {
			int request = mRequestQueue.front();
			mRequestQueue.erase(mRequestQueue.begin());
			lock.unlock();  // Unlock to allow other consumers to work
			
			std::cout << "Processing " << request << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			std::cout << "Completed " << request << std::endl;
			--mRemainingRequestCount;
			notify(request, mRemainingRequestCount, objectToNotify);
		}
	}
}
