#pragma once




namespace zq {


#define CLASS_MODULE_INIT(T)  public: static const char* getName() {return #T;}
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

