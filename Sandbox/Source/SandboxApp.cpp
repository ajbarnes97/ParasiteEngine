#include "ParasiteEngine.h"


class CSandbox : public Parasite::CApplication
{
public:
	CSandbox()
	{

	}

	~CSandbox()
	{

	}
};

Parasite::CApplication* Parasite::CreateApplication()
{
	return new CSandbox();
}