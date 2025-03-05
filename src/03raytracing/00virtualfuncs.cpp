#include <iostream>

class Food {
public:
    virtual void eat() = 0;
};

class JunkFood : public Food {
public:
    void eat() override;
};

void JunkFood::eat() {
    cout << "Wow you shouldn't eat me! I'm terrible for you!";
}

class Carrot : public Food {
public:
    void eat() override;
};

void Carrot::eat() {
    cout << "I'm healthy! Not as much fun as cake though...";
}

int main() {
    JunkFood jf;
    jf.eat();

    Carrot c;
    c.eat();

    Food* p = &jf;
    p->eat(); // if p is pointing to junkfood, call JunkFood::eat() if p pointing to carrot call Carrot::eat()
    p = &c;
    p->eat();


}
