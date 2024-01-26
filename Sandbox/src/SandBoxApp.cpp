
#include <ForByte.h>

class SandBox : public ForByte::Application {
public:
	SandBox() 
	{

	}
	~SandBox() 
	{

	}
};

ForByte::Application* ForByte::CreateApplication() 
{
	return new SandBox();
}