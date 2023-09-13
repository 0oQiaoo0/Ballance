#include "ThreadPool.h"
ThreadPool thread_pool(6);

void ThreadPool::Stop()
{
    thread_pool.StopInternal();
}

void ThreadPool::AddTask(Task&& task)
{
    thread_pool.AddTaskInternal(task);
}

void ThreadPool::AddTask(Task& task)
{
    thread_pool.AddTaskInternal(task);
}
