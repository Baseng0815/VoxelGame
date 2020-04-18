#include "../include/Application.h"

#include <mcheck.h>

int main(int argc, char **argv) {
    mtrace();
    Application application;

    application.run();
    muntrace();
}
