#pragma once
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

#include <vector>
#include <stack>
#include <queue>
#include <string>

// Element type constants
const int  INVALID_CHAR = -1;
const int  OPERAND = 1;
const int  BINARY_OPERATOR = 2;
const int  UNARY_OPERATOR = 3;
const int  OPEN_PARENDS = 4;
const int  CLOSE_PARENDS = 5;

// expression elements
struct Element
{
	std::string  item;        //string representing the element, either a set of numbers or a mathmatical symbel
	unsigned int position;    //start position in the string for the element
	int          type;        //the type of element it is, a number (operand), a operator ( multiplication sign, etc)
	int          precedence;  //order of precendence of 
};

class Expression
{
private:
	//Extract numeric(integers) values from the Expression string.Create an Operand Element type assigning the relative properties for the Element.
	Element getOperand(std::string v_strExpression, bool &r_bolNegative, unsigned int v_untSize, int v_intNegativeSignPosition, unsigned int &r_untIndex);

	//Break up the expression into individual elements (operators, operands, parenthises, etc.) assigning the 
	//relative properties for the individual elements (precidence, starting position, value, type)
	std::vector<Element> parse(std::string v_strExpression);

	//Check to see if the expression that was given has any error that can be detected before we attempt to evaluate it.
	bool validate(std::vector<Element> &r_vecElements);

	//Convet the expression from infix notation to post fix notation.  
	std::queue<Element> convertToPostfix(std::vector<Element> &r_vecElements);

	//Compute the results of a binary operation on two integer values
	int  computeBinaryOperation(int v_intValue1, int v_intValue2, std::string v_strOperator);

	//Compute the results of a unary operation on an integer values
	int  computeUnaryOperation(int v_intValue1, std::string v_strOperator);

	//Compute the value of a postFix expression. 
	int  computeResults(std::queue<Element> &r_queElements);

public:
	Expression();
	~Expression();

	int  evaluate(std::string  v_strExpression);
};

