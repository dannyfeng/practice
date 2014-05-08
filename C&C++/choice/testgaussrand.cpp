#include <iostream>
#include "gaussrand.h"

using namespace std;

int main(void)
{
    for (int i = 0; i < 1000; ++i)
        cout << gaussrand() << endl;
}

