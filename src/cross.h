#ifndef CROSS
#define CROSS 

#include <iostream>

#ifdef _WIN32
    #define __WINDOWS__OS__
#elif __linux__
    #define __LINUX__OS__
#endif

#endif // CROSS
