#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool {
    private:
        typedef std::function<void()> task_type;
        int _maxThreads = 4; // Nombre maximum de threads possible.

        std::vector<std::thread> _workers;
        std::mutex _mutex;
        std::condition_variable _cv;

        std::queue<task_type> _tasks;
        bool _isStopSignal = false;

    public:
        ThreadPool(int maxThreads);
        ThreadPool(const ThreadPool& tp) : _maxThreads(tp._maxThreads) { };

        void add(task_type task);
        void start();
        void stop();
};