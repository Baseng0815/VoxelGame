#include "../include/Application.h"

#ifndef WIN32
#include <mcheck.h>

int main(int argc, char **argv) {
    mtrace();
    Application application;

    application.run();
    muntrace();
}

#else 
int main(int argc, char** argv) {    
    Application application;

    application.run();    
}
#endif
