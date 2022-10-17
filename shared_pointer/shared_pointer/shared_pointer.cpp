#include <iostream>

using namespace std;


struct Person {
    int id;
    const char* name;
};


class SP {
private:
    class RC {
    private:
        int count;

    public:
        RC() : count{ 1 } {}

        void AddRef() {
            ++count;
        }

        int Release()
        {
            return --count;
        }
    };

    Person* ptr;  // raw pointer
    RC* reference;

public:
    SP();
    SP(Person* ptr);
    SP(const SP& a);
    SP(SP&& a);
    ~SP();

    SP& operator=(const SP& a);
    SP& operator=(SP&& a);

    Person& operator*() const;
    Person* operator->() const;
};


SP::SP() : ptr{ nullptr }, reference(nullptr) {

}


SP::SP(Person* ptr) : ptr{ ptr }, reference(new RC) {
    cout << "ctor\n";
}


SP::SP(const SP& a) : ptr{ a.ptr }, reference(a.reference) {
    reference->AddRef();
    cout << "copy ctor\n";
}


SP::SP(SP&& a) : ptr{ a.ptr }, reference(a.reference) {
    a.ptr = nullptr;
    a.reference = nullptr;
    cout << "move ctor\n";
}


SP::~SP() {
    if (reference != nullptr && reference->Release() == 0) {
        delete ptr;
        cout << "deleted\n";
    }
}


SP& SP::operator=(const SP& a) {
    // 기존 것을 놓는다.
    if (reference != nullptr && reference->Release() == 0) {
        delete ptr;
        cout << "deleted2\n";
    }

    // 새 것으로 덮어쓴다.
    ptr = a.ptr;
    reference = a.reference;
    reference->AddRef();

    cout << "copy operator=\n";

    return *this;
}


SP& SP::operator=(SP&& a) {
    // 기존 것을 놓는다.
    if (reference != nullptr && reference->Release() == 0) {
        delete ptr;
    }

    ptr = a.ptr;
    a.ptr = nullptr;

    reference = a.reference;
    a.reference = nullptr;

    cout << "move operator=\n";

    return *this;
}


Person& SP::operator*() const {
    return *ptr;
}


Person* SP::operator->() const {
    return ptr;
}


int main() {
    SP ptr{ new Person };

    {
        SP ptr2 = ptr;
        SP ptr3 = std::move(ptr);

        SP ptr4;
        ptr4 = ptr2;

        SP ptr5;
        ptr5 = std::move(ptr3);

        ptr = ptr4;
    }

    SP ptr6 = ptr;
}
