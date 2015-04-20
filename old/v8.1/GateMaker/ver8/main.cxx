
#include "MainWindow.h"


int main(int argc, char ** argv)	{

	TApplication *app = new TApplication("app",0,0);

	
	//MainWindow *mw;

	new MainWindow(400,200);
		
	app->Run();
	
	return 0;

}




