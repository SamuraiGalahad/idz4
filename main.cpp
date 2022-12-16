#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <semaphore.h>
#include <fstream>
#include <iostream>
#include "Visitor.h"

#ifdef _WIN32
#include <windows.h>
void sleep(unsigned milliseconds)
{
    Sleep(milliseconds);
}
#else
#include <unistd.h>
void sleep(unsigned milliseconds)
{
    usleep(milliseconds * 1000);
}
#endif

// семафоры для картин и галлереи
sem_t sem0, sem1, sem2, sem3, sem4, sem5;

// 5 процессов для картин
// 1й
void FirstPainting() {
    sem_wait(&sem1);
    sleep(100);
    sem_post(&sem1);
}
// 2й
void SecondPainting() {
    sem_wait(&sem2);
    sleep(100);
    sem_post(&sem2);
}
// 3й
void ThirdPainting() {
    sem_wait(&sem3);
    sleep(100);
    sem_post(&sem3);
}
// 4й
void ForthPainting() {
    sem_wait(&sem4);
    sleep(100);
    sem_post(&sem4);
}
// 5й
void FifthPainting() {
    sem_wait(&sem5);
    sleep(100);
    sem_post(&sem5);
}

// переменные для работы программы
bool dataType;
std::ofstream ofstream;
std::ifstream ifstream;

// функция вывода в зависимости от типа вывода
void print(std::string current_string) {
    if (!dataType) {
        if (ofstream.is_open()) {
            ofstream << current_string;
        }
    }
    if (dataType) {
        std::cout << current_string;
    }
}

// функция ввода в зависимости от типа вывода
std::string get_string() {
    if (dataType) {
        std::string input;
        std::cin >> input;
        return input;
    } else {
        std::string input;
        if (ifstream.is_open()) {
            ifstream >> input;
        }
        return input;
    }
}


// процесс вахтера
void *helpByVahter(void *arg) {
    bool flag = false;
    int current_visitor_id = *((int *) (arg));
    sem_wait(&sem0);
    print("Vahter said, that visitor " + std::to_string(current_visitor_id) + " now in gallery!\n");
    while (!flag) {
        int type = (rand()) % 6;

        if (type == 0) {
            sem_post(&sem0);
            print("Visitor " + std::to_string(current_visitor_id) + " not in gallery now!\n");
            break;
        } else {
            print("Visitor " + std::to_string(current_visitor_id) + " in front of  " + std::to_string(type) +
                " painting\n");
            switch (type) {
                case 1: FirstPainting(); break;
                case 2: SecondPainting(); break;
                case 3: ThirdPainting(); break;
                case 4: ForthPainting(); break;
                case 5: FifthPainting(); break;
            }
        }
    }
}

// инициализация семафоров
void init_sem() {
    sem_init(&sem0, 0, 50);
    sem_init(&sem1, 0, 10);
    sem_init(&sem2, 0, 10);
    sem_init(&sem3, 0, 10);
    sem_init(&sem4, 0, 10);
    sem_init(&sem5, 0, 10);
}

// закрытие семафоров
void destroy_sem() {
    sem_destroy(&sem0);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);
    sem_destroy(&sem4);
    sem_destroy(&sem5);
}

// главный процесс выполнения программы
int main(int argc, char *argv[]) {
    std::vector<Visitor> data;
    ofstream.open("output.txt");
    ifstream.open("input.txt");
    int count_v = 50;
    int command;
    bool wrong_command = true;
    // первый ввод из консоли
    while (wrong_command) {
        std::cout << "Tap '1', to make input/output from/in file.\n";
        std::cout << "Tap '2', to get input/output from/in console.\n";
        std::cout << ">>";
        try {
            std::string str_command;
            std::cin >> str_command;
            command = std::stoi(str_command);
            if (command != 1 && command != 2) {
                throw std::exception();
            }
        } catch (std::exception &er) {
            continue;
        }
        wrong_command = false;
    }

    switch (command) {
        case 1:
            dataType = false;
            break;
        case 2:
            dataType = true;
            break;
    }

    wrong_command = true;
    // второй ввод из консоли
    while (wrong_command) {
        std::cout << "Type '1', to make random count of visitors.\n";
        std::cout << "Type '2', to get count of visitors from console.\n";
        std::cout << "Type '3', to get visitors from argv.\n";
        std::cout << ">>";
        try {
            std::string str_command;
            std::cin >> str_command;
            command = std::stoi(str_command);
            if (command != 1 && command != 2 && command != 3) {
                throw std::exception();
            }
        } catch (std::exception &er) {
            continue;
        }
        wrong_command = false;
    }

    switch (command) {
        case 1:
            count_v = rand() % 90 + 20;
            break;
        case 2:
            count_v = std::stoi(get_string());
            break;
        case 3:
            count_v = std::stoi(argv[1]);
            break;
    }

    init_sem();
    // помощь вахтера для каждого посетителя
    for (int i = 0; i < count_v; ++i) {
        data.push_back(Visitor(i));
        data[i].addHelpFromVahter(helpByVahter);
    }
    // ожидание
    for (int i = 0; i < count_v; ++i) {
        pthread_join(data[i].visitor_thread, nullptr);
    }

    destroy_sem();
    ofstream.close();
    ifstream.close();
    return 0;
}