// Project_One.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Expression.h"
#include <iostream>
#include <math.h>

int main()
{
	Expression x;
	try {
		//x.evaluate("++((33+6)/3)");
		//x.evaluate("1/0");
		//x.evaluate("++++13");
		x.evaluate("++++2-5*(3^2)");
		//int y = 0;
		//y = pow(2, -4);
		//std::cout << y << std::endl;
	}
	catch(std::string msg){
		std::cout << msg << std::endl;
	}

	system("pause");
    return 0;
}

