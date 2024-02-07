#include <iostream>

class MyClass {
public:
    MyClass(int value) : data(new int(value)) {
        std::cout << "Constructor: " << *data << std::endl;
    }

    ~MyClass() {
        std::cout << "Destructor: " << *data << std::endl;
        delete data;
    }

    MyClass(const MyClass& other) : data(new int(*(other.data))) {
        std::cout << "Copy Constructor: " << *data << std::endl;
    }

    MyClass(MyClass&& other) noexcept : data(nullptr) {
        std::swap(data, other.data); //take ownership
        std::cout << "Move Constructor" << std::endl;
    }

    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            int* newData = new int(*(other.data));
            delete data;
            data = newData;
            std::cout << "Copy Assignment Operator: " << *data << std::endl;
        }
        return *this;
    }
    
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            std::swap(data, other.data);
            delete other.data;
            other.data = nullptr;
            std::cout << "Move Assignment Operator: " << *data << std::endl;
        }
        return *this;
    }

    int getVal() const {
        return *data;
    }

    void setVal(int value) {
        *data = value;
        std::cout << "    Value set to: " << *data << std::endl;
    }

private:
    int* data;
};

// Function using MyClass&
void one_and(const MyClass& obj) {
    std::cout << "Using const MyClass&: " << obj.getVal() << std::endl;
}

// Function using MyClass&&
void two_and(MyClass&& obj) {
    std::cout << "Using MyClass&&: " << obj.getVal() << std::endl;
}

void modify(MyClass& obj) {
    int newVal = obj.getVal() + 1;
    std::cout << "Modified to +1: " << std::endl;
    obj.setVal(newVal);
}


int main() {
    MyClass obj(11);

    one_and(obj);

    modify(obj);

    two_and(MyClass(22));    
    
    std::cout.flush();
    
    return 0;
}
