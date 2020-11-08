#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <iomanip>
#include "utils.h"

class ThreadPool {
    private:
        typedef std::function<void()> task_type;
        int _maxThreads;

        std::vector<std::thread> _workers;
        std::mutex _mutex;
        std::mutex _mutexDebug;
        std::condition_variable _cv;

        std::queue<task_type> _tasks;
        bool _isStopSignal;

    public:
        ThreadPool();
        ThreadPool(int maxThreads);
        ThreadPool(const ThreadPool& tp) : _maxThreads(tp._maxThreads) { };
        ~ThreadPool();

        void monitorPool();
        void add(task_type task);
        void start();
        void stop();
};