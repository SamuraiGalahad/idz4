//
// Created by Николай on 16.12.2022.
//
#include <pthread.h>
#include "stdlib.h"
#include <semaphore.h>

#ifndef IDZ4_VISITOR_H
#define IDZ4_VISITOR_H


class Visitor {
public:
    int visitor_id;
    pthread_t visitor_thread;

    Visitor(int id) {
        visitor_id = id;
        visitor_thread = pthread_t();
    }

    void addHelpFromVahter(void *(*func)(void *)) {
        pthread_create(&visitor_thread, nullptr, func, &visitor_id);
    }
};


#endif //IDZ4_VISITOR_H
