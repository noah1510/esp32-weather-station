#include <drogon/drogon.h>
#include "helloController.hpp"

#include <iostream>

using namespace drogon;

int main(){
    app().loadConfigFile("./config.json").run();
}
