#include "MyApplication.h"



int main() 
{
	MyApplication* myApp = new MyApplication();
	if(myApp->Init() == 1)
	{
		while(myApp->Update() == true)
			myApp->Draw();
		myApp->Close();
	}
	
	delete myApp;
	return 0;
}