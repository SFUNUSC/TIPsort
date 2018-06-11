#include "MainWindow.h"

int main(int argc, char ** argv){

  // int ac;
  // char* av[10];
  // TApplication *app = new TApplication("app",&ac,av);
  TApplication *app = new TApplication("app",0,0);
  
  // MainWindow *mw;
  
  new MainWindow(1000,700);
  
  app->Run();
  
  return 0;
}




