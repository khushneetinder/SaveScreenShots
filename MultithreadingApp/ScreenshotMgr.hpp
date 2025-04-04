//
//  ScreenshotMgr.hpp
//  MultithreadingApp
//
//  Created by Khushneet Inder on 4/4/2025.
//

#ifndef ScreenshotMgr_hpp
#define ScreenshotMgr_hpp

#include <stdio.h>
#include <vector>
#include <thread>

typedef int Screenshot;
typedef std::vector<Screenshot> SSQueue;
typedef std::function<void(int current, int remainingCount, void* objectToUpdate)> CallBack;

class ScreenshotMgr {
public:
	ScreenshotMgr();
	~ScreenshotMgr();
	void Take(int request, CallBack notify, void* objectToUpdate);
	void Save(CallBack notify, void* objectToNotify);
	int GetRemainingRequestCount() const {return mRemainingRequestCount;}
private:
	Screenshot mShotCount;
	std::mutex mMutex;	// to protect queue
	std::atomic<int> mRemainingRequestCount;
	SSQueue mRequestQueue;
	std::condition_variable mQueueCondition;
	std::thread mWorker;
};

#endif /* ScreenshotMgr_hpp */

