#pragma once
#include <list>
#include <mutex>

#include "macro.h"

template<typename T>
class SyncQueue
{
public:
    SyncQueue(int maxSize) :maxSize(maxSize), needStop(false) {}
    void Put(const T& x)
    {
        Add(x);
    }
    void Put(T&& x)
    {
        Add(std::forward<T>(x));
    }
    void Take(std::list<T>& list)
    {
        std::unique_lock<std::mutex> locker(mMutex);
        mNotEmpty.wait(locker, [this] {return needStop || NotEmpty(); });
        if (needStop)return;
        list = std::move(mQueue);
        mNotFull.notify_one();
    }
    void Take(T& t)
    {
        std::unique_lock<std::mutex> locker(mMutex);
        mNotEmpty.wait(locker, [this] {return needStop || NotEmpty(); });
        if (needStop)return;
        t = mQueue.front();
        mQueue.pop_front();
        mNotFull.notify_one();
    }
    void Stop()
    {
        {
            std::lock_guard<std::mutex> locker(mMutex);
            needStop = true;
        }
        mNotFull.notify_all();
        mNotEmpty.notify_all();
    }
    bool Empty()
    {
        std::lock_guard<std::mutex> locker(mMutex);
        return mQueue.empty();
    }
    bool Full()
    {
        std::lock_guard<std::mutex> locker(mMutex);
        return mQueue.size() >= maxSize;
    }
    size_t Size()
    {
        std::lock_guard<std::mutex> locker(mMutex);
        return mQueue.size();
    }
    int Count()
    {
        return mQueue.size();
    }

private:
    bool NotFull()const
    {
        bool full = mQueue.size() >= maxSize;
        if (full)
        {
            //LOG_WARN("buffer full please wait");
        }
        return !full;
    }
    bool NotEmpty()
    {
        bool empty = mQueue.empty();
        if (empty)
        {
            // LOG_WARN("buffer empty ,please wait,thread id", std::this_thread::get_id());
        }
        return !empty;
    }
    template<class  F>
    void Add(F&& f)
    {
        std::unique_lock<std::mutex> locker(mMutex);
        mNotFull.wait(locker, [this] {return needStop || NotFull(); });
        if (needStop)return;
        mQueue.push_back(std::forward<F>(f));
        mNotEmpty.notify_one();
    }
    std::list<T> mQueue;
    std::mutex mMutex;
    std::condition_variable mNotEmpty;
    std::condition_variable mNotFull;
    int maxSize;
    bool needStop;
};
