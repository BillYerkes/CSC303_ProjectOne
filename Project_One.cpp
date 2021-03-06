//###########################################################################################################################
// CS 303
// Project One
// Bill Yerkes         wy2n6@mail.umkc.edu
// Aditya Rai		   adityarai@mail.umkc.edu
// David Tooley        dwtvf2@mail.umkc.edu
//
// PROBLEM: Create an infix expression parser.  It need to support the following arithmetic and logical operators:
//
//          Operator				Precedence			Example 
//			! //logical not				8				! 1 // = 0 (false) 
//			++ //prefix increment		8				++2 //3 
//			-- //prefix decrement		8				--2 //1 
//			- //negative				8				-1  //-1 
//			^ //power					7				2^3 // 8 
//			*, / , %  //arithmetic		6				6 * 2 // 12 
//			+, - //arithmetic			5				6 – 2 //4 
//			>, >= , <, <= //comparison	4				6 > 5 // 1(true) ==, 
//			!= //equality comparison	3				6!=5 // 1(true) 
//			&&  //logical and			2				6>5 && 4>5 //0(false) 
//			|| //logical OR				1				1 || 0 //1 (true) 
//
// The expression will be passed in as a string and the solution must be able to handle irrelivent white space
// such as 1+2 is the same as 1 + 2.  The solution must also check for invalid expression, such as mismatched
// parenthesises and division by zero.  The solution must evaluate the expression and return the results.
// Major assumption is that all number are integers and the results will be an integer.  User interface is not
// required for this project.
//
// 
// ERROR HANDLING: Error from the class (Expression) are raised to the calling modual.  Error type is string.
//
// OTHER COMMENTS:
//
// l_  : Local Varible
// v_  : passed by value
// r_  : passed by reverence
// str : string
// chr : char
// int : integer
// unt : unsigned integer
// bol : Boolean
// que : queue
// vec : vector
// stk : stack
// udt : user defined type
//
//############################################################################################################################

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

