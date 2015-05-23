#include "app.h"
#include "config.h"

#include <iostream>

int main(int argc, char *argv[])
{
	App app;

	app.setConfig(Config());
	if (app.refreshIP())
		std::cout << "IP is: " << app.ip() << std::endl;
	else
		std::cout << app.lastError() << std::endl;

	return 0;
}
