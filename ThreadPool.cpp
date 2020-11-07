#include "ThreadPool.h"

ThreadPool::ThreadPool(): ThreadPool(std::thread::hardware_concurrency()) {
}

ThreadPool::ThreadPool(int maxThreads) {
     // Nombre maximum de threads possible.
    _maxThreads = maxThreads;
    _isStopSignal = false;
    start();
}

ThreadPool::~ThreadPool() {
    // stop();
}

void ThreadPool::add(task_type task) {
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _tasks.push(task);
    }

    _cv.notify_one();
}

void ThreadPool::monitorPool() {
    while(true) {
        task_type task;
        {
            // Bloquer l'exécution du thread tant qu'il n'y a pas de signal
            // d'arrêt ou de tâches à exécuter.
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait(lock, [this]{ return !_tasks.empty() || _isStopSignal;});

            // Quitter l'exécution du thread si on reçoit le signal d'arrêt
            // ET qu'il n'y a plus de tâches à exécuter.
            if (_isStopSignal && _tasks.empty()) {
                return;
            }

            // Saisir la prochaine tâche sur la pile d'exécution et
            // l'enlever.
            task = _tasks.front();
            _tasks.pop();
        }

        // Exécuter la tâche.
        double s = stopwatch([task] { task(); });

        {
            std::thread::id threadId = std::this_thread::get_id();
            std::lock_guard<std::mutex> lock(_mutexDebug);
            printf("Thread 0x%x: END TASK (%.8fs)\n", threadId, s);
        }
    }
}

void ThreadPool::start() {
    for(int i = 0; i < _maxThreads; i++) {
        // Ajouter le thread ouvrier dans le "thread pool".
        _workers.push_back(std::thread(&ThreadPool::monitorPool, this));
    }
}

void ThreadPool::stop() {
    {
        // Activer tous les threads en signalant un arr�t.
        std::unique_lock<std::mutex> lock(_mutex);
        _isStopSignal = true;
    }

    _cv.notify_all();

    // Attendre que tous les threads ouvriers terminent leur travail.
    for(std::thread &t : _workers) {
        t.join();
    }

    _workers.clear();
}