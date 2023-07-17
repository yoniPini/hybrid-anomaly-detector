#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
  this->dio = dio;
}

void CLI::start(){
  Command *menu = new Menu(dio);
  menu->execute();
}

CLI::~CLI() {
}

