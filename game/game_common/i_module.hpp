#pragma once


namespace zq {


#define INIT_MODULE_NAME(T)  public: static const char* getName() {static const char* name = #T;return name;}
class IModule
{
public:

    IModule() {}
	virtual ~IModule() {}

	virtual bool init()
	{
		return true;
	}

	virtual bool update()
	{
		return true;
	}

	virtual bool finalize()
	{
		return true;
	}
};

}

