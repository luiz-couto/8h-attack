#pragma once

template <typename T, int SIZE = 10>
class Vector {
    private:
    T *arr;
    int maxSize = SIZE;
    int size = 0;
    
    public:
    Vector() {
        this->arr = new T[maxSize];
    }

    ~Vector() {
        delete[] arr;
    }

    Vector(const Vector& other) {
        this->maxSize = other.maxSize;
        this->size = other.size;

        this->arr = new T[this->maxSize]; 
        std::memcpy(this->arr, other.arr, size * sizeof(T));
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this; 
        }

        delete[] this->arr;

        this->maxSize = other.maxSize;
        this->size = other.size;

        this->arr = new T[this->maxSize];
        std::memcpy(this->arr, other.arr, size * sizeof(T));
        return *this;
    }

    void add(T item) {
        if (size < maxSize) {
			this->arr[size++] = item;
            return;
        }

        T* newarray;
        this->maxSize *= 2;
        std::cout << "Growing array to " << maxSize << std::endl;

        newarray = new T[maxSize]; // should check if there was enough space really
        memcpy(newarray, this->arr, size * sizeof(T));
        delete[] this->arr;
        this->arr = newarray;
        this->arr[size++] = item;
    }
    
    void display() {
		for (int i = 0; i < size; i++)
			std::cout << this->arr[i] << '\t';
		std::cout << std::endl;
	}

    int getSize() {
        return this->size;
    }

	T& operator[] (unsigned int index) {
		// lazy implementation should test bounds really 
		return this->arr[index];
	}

};