#pragma once

#include "Application.h" 

#ifdef PE_PLATFORM_WINDOWS


extern Parasite::CApplication* CreateApplication();

int main(int argc, char** argv)
{
	Parasite::CLog::Init();

	auto app = Parasite::CreateApplication();
	app->Run();
	delete app;
}
#endif