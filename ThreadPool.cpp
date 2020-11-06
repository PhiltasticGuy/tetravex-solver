#include "ThreadPool.h"

ThreadPool::ThreadPool(int maxThreads) {
    _maxThreads = maxThreads;
    start();
}

void ThreadPool::add(task_type task) {
    std::unique_lock<std::mutex> lock(_mutex);

    _tasks.push(task);

    _cv.notify_one();
}

void ThreadPool::start() {
    for(int i = 0; i < _maxThreads; i++) {
        auto monitor = [this] {
            while(true) {
                // Bloquer l'exécution du thread tant qu'il n'y a pas de signal
                // d'arrêt ou de tâches à exécuter.
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this]{ return _isStopSignal || !_tasks.empty();});

                // Quitter l'exécution du thread si on reçoit le signal d'arrêt
                // ET qu'il n'y a plus de tâches à exécuter.
                if (_isStopSignal && _tasks.empty()) {
                    return;
                }

                // Saisir la prochaine tâche sur la pile d'exécution et
                // l'enlever.
                task_type task = _tasks.front();
                _tasks.pop();

                // Exécuter la tâche.
                task();
            }
        };

        // Ajouter le thread ouvrier dans le "thread pool".
        _workers.push_back(std::thread(monitor));
    }
}

void ThreadPool::stop() {
    // Activer tous les threads en signalant un arr�t.
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _isStopSignal = true;
        _cv.notify_all();
    }

    // Attendre que tous les threads ouvriers terminent leur travail.
    for(int i = 0; i < _maxThreads; i++) {
        _workers[i].join();
    }
}