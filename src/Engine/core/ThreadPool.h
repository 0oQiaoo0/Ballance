#pragma once
#include <functional>
#include <list>
#include <thread>
#include <functional>
#include <atomic>
#include "SyncQueue.h"

const int MaxTaskCount = 100;

class ThreadPool
{
public:
    using Task = std::function<void()>;
    ThreadPool(unsigned int numThread = std::thread::hardware_concurrency()) :mQueue(MaxTaskCount), num_thread_(numThread)
    {
        Start(numThread);
    }
    ~ThreadPool(void)
    {
        Stop();
    }
    static  void Stop();
    static  void AddTask(Task&& task);
    static  void AddTask(Task& task);
private:
    void StopInternal()
    {
        std::call_once(mFlag, [this] {StopThreadGroup(); });
    }
    void AddTaskInternal(Task&& task)
    {
        mQueue.Put(std::forward<Task>(task));
    }
    void AddTaskInternal(Task& task)
    {
        mQueue.Put(task);
    }
    void Start(unsigned int numThread)
    {
        mRunning = true;
        for (unsigned int i = 0; i < numThread; ++i)
        {
            mThreadGroup.push_back(
                std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
        }
    }
    void RunInThread()
    {
        while (mRunning)
        {
            std::list<Task> list;
            mQueue.Take(list);
            for (auto& task : list)
            {
                if (!mRunning)return;
                task();
            }
        }
    }
    void StopThreadGroup()
    {
        mQueue.Stop();
        mRunning = false;
        for (auto thread : mThreadGroup)
        {
            if (thread)thread->join();
        }
        mThreadGroup.clear();
    }
    std::list<std::shared_ptr<std::thread>> mThreadGroup;
    SyncQueue<Task> mQueue;
    std::atomic_bool mRunning;
    std::once_flag mFlag;
    int num_thread_;
};
extern ThreadPool thread_pool;