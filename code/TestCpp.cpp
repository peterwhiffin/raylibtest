#include <cstdio>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

class WereBack
{
public:
    float speed = 1.0f;
    float height = 2.0f;
    bool *shouldExit = NULL;

    WereBack(bool *shouldExit)
    {
        this->shouldExit = shouldExit;
    };

    virtual void Update()
    {
        if (_kbhit() && _getch() == 27)
        {
            *shouldExit = true;
            // exit(-1);
        }
    }
};

class AreWeBack : WereBack
{
public:
    AreWeBack::AreWeBack(bool *shouldExit) : WereBack(shouldExit)
    {
    }
    
     void Update() override {
        WereBack::Update();
    }
};


int main()
{
    printf("hello");
    bool shouldExit = false;
    AreWeBack backObj(&shouldExit);

    while (!shouldExit)
    {
        backObj.Update();
    }

    printf("Exiting");
    return 0;
}
