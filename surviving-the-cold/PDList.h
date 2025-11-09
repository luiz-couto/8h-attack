#pragma once
#include <iostream>

#define DEFAULT_MAX_SIZE 100

// PDList - Pseudo Dynamic List
template<typename T, int MAX_SIZE = DEFAULT_MAX_SIZE>
class PDList {
    private:
    T* list[MAX_SIZE];
    int nextFreeIdx = 0;
    int iteratorIdx = 0;
    bool isIterating = false;

    public:
    PDList() {
        for (int i=0; i<MAX_SIZE; i++) {
            list[i] = nullptr;
        }
    }

    void add(T *item) {
        if (nextFreeIdx == MAX_SIZE) {
            return;
        }
        list[nextFreeIdx] = item;
        nextFreeIdx++;
    }
    
    void deleteByIdx(int idx) {
        delete list[idx];
        list[idx] = nullptr;
        for (int i = idx + 1; i <= nextFreeIdx; i++) {
            list[i - 1] = list[i];
        }
        nextFreeIdx--;

        if (isIterating) {
            iteratorIdx--;
        }
    }

    void display(bool showNulls = true) {
        for (int i = 0; i < MAX_SIZE; i++) {
            if (list[i] != nullptr) {
                std::cout << *(list[i]) << " ";
                continue;
            }
            if (showNulls) {
                std::cout << "nullptr ";
            }
        }
        std::cout << std::endl;
    }

    template<typename Func>
    void forEach(Func func) {
        isIterating = true;
        for (iteratorIdx = 0; iteratorIdx < nextFreeIdx; iteratorIdx++) {
            func(*list[iteratorIdx], iteratorIdx);
        }
        isIterating = false;
    }

    T* at(unsigned int idx) {
        return this->list[idx];
    }

    int size() {
        return this->nextFreeIdx;
    }

    template<typename Func>
    void sort(Func compareFunc) {
        for (int i = 0; i < nextFreeIdx - 1; i++) {
            for (int j = 0; j < nextFreeIdx - i - 1; j++) {
                if (!compareFunc(*list[j], *list[j + 1])) {
                    T* temp = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = temp;
                }
            }
        }
    }

    ~PDList() {
        for (int i = 0; i < nextFreeIdx; i++) {
            delete list[i];
        }
    }
};