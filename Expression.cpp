#include "stdafx.h"
#include "Expression.h"

#include <iostream>
#include <math.h>


//#include <exception>


//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################

Element Expression::getOperand(std::string v_strExpression, bool &r_bolNegative, unsigned int v_untSize, int v_intNegativeSignPosition, unsigned int &r_untIndex)
{
	bool l_bolNumber = true;
	Element l_udtElement;

	//if we have deteremined the number should be negative, we will add a minus sign to the front of the number
	if (r_bolNegative) {
		l_udtElement.item = "-";
		l_udtElement.position = v_intNegativeSignPosition;
		l_udtElement.type = OPERAND;
		r_bolNegative = false;
	}
	else {
		l_udtElement.item = "";
		l_udtElement.position = r_untIndex;
		l_udtElement.type = OPERAND;
	}

	l_udtElement.item += v_strExpression[r_untIndex];

	//while we keep encountering number values, we will append it to operand (number)
	while (l_bolNumber) {

		if (r_untIndex < v_untSize - 1) {
			r_untIndex++;

			switch (v_strExpression[r_untIndex])
			{
			case '0': case '1': case '2': case '3':	case '4':
			case '5': case '6': case '7': case '8': case '9':
				l_udtElement.item += v_strExpression[r_untIndex];
				//r_untIndex++;
				break;
			default: //we do not have a number, we can exit this loop now.
				l_bolNumber = false;
				break;
			}
		}
		else { //we are at the end, exit the loop
			r_untIndex++;
			l_bolNumber = false;
		}
	} //end while

	l_udtElement.precedence = 0;
	return l_udtElement;
}



//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
std::vector<Element> Expression::parse(std::string v_strExpression)
{
	std::vector<Element> l_vecElements;
	unsigned int         l_untSize = 0;
	Element              l_udtElement;
	bool                 l_bolNegative = false;
	int                  l_intNegativeSignPosition = 0;
	char                 l_chrTemp = ' ';
	std::string          l_strTemp = "";
	std::string          l_strErrorMessage;
	bool                 l_bolValid = true;

	    // save the size of the string so we do not have to keep looking it up
		l_untSize = v_strExpression.length();
		unsigned int i = 0;

		// while we still have char to process and we have not encountered an invalid sequence of chars
		while ((i < l_untSize) && (l_bolValid)){


			switch (v_strExpression[i])
			{
			//Number
			case '0': case '1': case '2': case '3':	case '4':
			case '5': case '6': case '7': case '8': case '9':
				l_udtElement = getOperand(v_strExpression, l_bolNegative, l_untSize, l_intNegativeSignPosition, i);
				l_vecElements.push_back(l_udtElement);
				break;

			//Open Parend
			case '(':
				l_udtElement.item = v_strExpression[i];
				l_udtElement.position = i;
				l_udtElement.type = OPEN_PARENDS;
				l_udtElement.precedence = 9;

				l_vecElements.push_back(l_udtElement);
				i++;

				break;

			//close Parend
			case ')':
				l_udtElement.item = v_strExpression[i];
				l_udtElement.position = i;
				l_udtElement.type = CLOSE_PARENDS;
				l_udtElement.precedence = 9;

				l_vecElements.push_back(l_udtElement);
				i++;

				break;

			//Power operator
			case '^':
				l_udtElement.item = v_strExpression[i];
				l_udtElement.position = i;
				l_udtElement.type = BINARY_OPERATOR;
				l_udtElement.precedence = 7;

				l_vecElements.push_back(l_udtElement);
				i++;

				break;

			//Multiplication and division 
			case '*': case '/': case '%':

				l_udtElement.item = v_strExpression[i];
				l_udtElement.position = i;
				l_udtElement.type = BINARY_OPERATOR;
				l_udtElement.precedence = 6;

				l_vecElements.push_back(l_udtElement);
				i++;

				break;

			//Addition or increment
			case '+':
				if (i < l_untSize - 1) {
					i++;
					if (v_strExpression[i] == '+') {
						l_udtElement.item = "++";
						l_udtElement.position = i - 1;
						l_udtElement.type = UNARY_OPERATOR;
						l_udtElement.precedence = 8;

						i++; // we used the second + sign, move to the next char
					}
					else {
						l_udtElement.item = "+";
						l_udtElement.position = i - 1;
						l_udtElement.type = BINARY_OPERATOR;
						l_udtElement.precedence = 5;
					}
				}
				else { // we are at the end
					l_udtElement.item = "+";
					l_udtElement.position = i;
					l_udtElement.type = BINARY_OPERATOR;
					l_udtElement.precedence = 5;
					i++; // move past the end, so we will exit the while loop
				}

				l_vecElements.push_back(l_udtElement);
				break;

			// Subtraction, decrement, or negative sign
			case '-':
				if (i < l_untSize - 1) { //are we at the end?
					i++;
					if (v_strExpression[i] == '-') { //do we have "--"
						l_udtElement.item = "--";
						l_udtElement.position = i - 1;
						l_intNegativeSignPosition = 0;
						l_udtElement.type = UNARY_OPERATOR;
						l_udtElement.precedence = 8;
						i++;

						l_vecElements.push_back(l_udtElement);
					}
					else {
						//starting with negative sign, turn negative flag on.
						if (l_vecElements.size() == 0) {
							l_intNegativeSignPosition = i - 1;
							l_bolNegative = true;
						}
						else { // we need to look at the previous element to determine what to do with the "-"
							switch (l_vecElements[l_vecElements.size() - 1].type)
							{
							case OPERAND: case INVALID_CHAR: case CLOSE_PARENDS:
								l_udtElement.item = "-";
								l_udtElement.position = i - 1;
								l_intNegativeSignPosition = 0;
								l_udtElement.type = BINARY_OPERATOR;
								l_udtElement.precedence = 5;

								l_vecElements.push_back(l_udtElement);
								l_bolNegative = false;
								break;

							case BINARY_OPERATOR: case UNARY_OPERATOR: case OPEN_PARENDS:
								l_intNegativeSignPosition = i - 1;
								if (l_bolNegative) {
									l_strErrorMessage = "Negative sign ambiguity @ char: " + std::to_string(i);
									throw l_strErrorMessage;
									//return;
								}
								l_bolNegative = true;

								break;

							default: //should not happen
								break;
							}
						}
					}
				}

				else { //we are at the end
					l_udtElement.item = "-";
					l_udtElement.position = i - 1;
					l_udtElement.type = BINARY_OPERATOR;
					l_udtElement.precedence = 5;

					l_vecElements.push_back(l_udtElement);
					i++;
				}

				break;

			// eqaulity comparison "==" (assignment "=" is not a valid option.)
			case '=':
				if (i < l_untSize - 1) {
					i++;
					if (v_strExpression[i] == '=') {
						l_udtElement.item = "==";
						l_udtElement.position = i;
						l_udtElement.type = BINARY_OPERATOR;
						l_udtElement.precedence = 3;
					}
					else {
						l_udtElement.item = "=";
						l_udtElement.position = i;
						l_udtElement.type = INVALID_CHAR;
						l_udtElement.precedence = 0;
						l_bolValid = false;
					}
						
				}
				else {
					l_udtElement.item = "=";
					l_udtElement.position = i;
					l_udtElement.type = INVALID_CHAR;
					l_udtElement.precedence = 0;
					l_bolValid = false;
				}
					
				l_vecElements.push_back(l_udtElement);
				i++;
				break;

			//Greater then, less then, greter then or equal, and less then or equal
			case '>': case '<': 

				l_udtElement.item = "";
				l_udtElement.item += v_strExpression[i];
				l_udtElement.position = i;
				l_udtElement.type = BINARY_OPERATOR;
				l_udtElement.precedence = 4;

				if (i < l_untSize - 1) {
					i++;
					if (v_strExpression[i] == '=')
						l_udtElement.item += '=';
					
				}
				else
					i++; // move past the end, so we will exit the while loop

				l_vecElements.push_back(l_udtElement);
				break;

			//logical NOT
			case '!':
				l_udtElement.item = "!";
				l_udtElement.position = i;

				if (i < l_untSize - 1) {
					i++;
					if (v_strExpression[i] == '=') {

						l_udtElement.item += '=';
						l_udtElement.type = BINARY_OPERATOR;
						l_udtElement.precedence = 3;

					}
					else {
						l_udtElement.type = UNARY_OPERATOR;
						l_udtElement.precedence = 8;

					}
				}
				else {
					l_udtElement.type = UNARY_OPERATOR;
					l_udtElement.precedence = 8;

					i++;
				}

				l_vecElements.push_back(l_udtElement);
				break;

			//Ligical AND and OR
			case '&': case '|':
				l_chrTemp = v_strExpression[i];
				if (i < l_untSize - 1) {
					i++;
					if (l_chrTemp == v_strExpression[i]) {

						if (v_strExpression[i] == '|')

							l_udtElement.item = "||";
						else
							l_udtElement.item = "&&";

						l_udtElement.position = i - 1;
						l_udtElement.type = BINARY_OPERATOR;
					}
					else {
						l_udtElement.position = i - 1;
						l_udtElement.type = INVALID_CHAR;
						l_udtElement.precedence = 0;
						l_bolValid = false;
					}

				}
				else {
					l_udtElement.position = i;
					l_udtElement.type = INVALID_CHAR;
					l_udtElement.precedence = 0;
					l_bolValid = false;

				}

				i++;
				l_vecElements.push_back(l_udtElement);
				break;

			//white space
			case ' ': case '\t': case '\n':
				i++;
				break;

			//All other char are not permitted
			default:
				l_udtElement.item = v_strExpression[i];
				l_udtElement.position = i;
				l_udtElement.type = INVALID_CHAR;
				l_vecElements.push_back(l_udtElement);
				l_bolValid = false;

				i++;
				break;

			}
		}

		return l_vecElements;

}

//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
bool Expression::validate(std::vector<Element> &r_vecElements)
{
	int          l_intParendCount = 0;
	unsigned int l_untSize = r_vecElements.size();
	bool         l_blnValid = true;
	std::string  l_strErrorMessage = "";
	unsigned int i = 0;
	int          l_intTemp = 0;

	//try {
		while ((i < l_untSize) && (l_blnValid)) {
			switch (r_vecElements[i].type)
			{
			case INVALID_CHAR:
				l_strErrorMessage = "Invalid element in expression @ char: " + std::to_string(r_vecElements[i].position);
				throw l_strErrorMessage;
				return false;
				break;

			case OPERAND:
				l_intTemp = std::stoi(r_vecElements[i].item);
				if ((l_intTemp == 0) && (i > 0)) {
					if (r_vecElements[i-1].item == "/")
						throw "Division by Zero @ char: " + std::to_string(r_vecElements[i].position);
					if (r_vecElements[i - 1].item == "%")
						throw "Modula division by Zero @ char: " + std::to_string(r_vecElements[i].position);

				}
				if (i > 0) {
					if (r_vecElements[i-1].type == OPERAND)
						throw "Two operands in a row @ char: " + std::to_string(r_vecElements[i].position);
				}

				break;

			case BINARY_OPERATOR:
				if (i == 0) {
					l_blnValid = false;
					throw "Expression can't start with a binary operator @ char: " + std::to_string(r_vecElements[i].position);
				}
				else if (r_vecElements[i - 1].type == BINARY_OPERATOR) {
					l_blnValid = false;
					throw "Two binary operators in a row @ char: " + std::to_string(r_vecElements[i].position);
				}
				else if (r_vecElements[i - 1].type == UNARY_OPERATOR) {
					l_blnValid = false;
					throw "A unary operand can't be folllowed by a binary operator @ char: " + std::to_string(r_vecElements[i].position);
				}
				if (i == (l_untSize - 1)) {
					throw "Expression cannot end in a binary operator @ char: " + std::to_string(r_vecElements[i].position);
				}

				break;
			case UNARY_OPERATOR:
				if (i == (l_untSize - 1)) {
					throw "Expression cannot end in an unary operator @ char: " + std::to_string(r_vecElements[i].position);
				}
				break;

			case CLOSE_PARENDS:
				l_intParendCount -= 1;
				if (l_intParendCount < 0) {
					l_blnValid = false;
					throw "Missing match for close parendthesis @ char: " + std::to_string(r_vecElements[i].position);
				}
				break;

			case OPEN_PARENDS:
				if (i == (l_untSize - 1)) {
					throw "Expression cannot end in with an Open perendathis @ char: " + std::to_string(r_vecElements[i].position);
				}
				l_intParendCount += 1;
				break;

			default:
				//This logically should not happen
				l_blnValid = false;
				throw "Invalid element in expression @ char: " + std::to_string(r_vecElements[i].position);

				break;

			} //end switch
			i++;
		} //end while

		return l_blnValid;
}

//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
std::queue<Element> Expression::convertToPostfix(std::vector<Element> &r_vecElements)
{
	std::queue<Element>  l_queElements;
	unsigned int l_untSize = r_vecElements.size();
	std::stack<Element>  l_stkWorkStack;

		for (unsigned int i = 0; i < l_untSize; i++) {
			// if we have a number, put it in the queue
			if (r_vecElements[i].type == OPERAND) {
				l_queElements.push(r_vecElements[i]);
			}
			else { //we have an operator
				//if we have a parend, push it onto the work stack
				if (r_vecElements[i].type == OPEN_PARENDS)
					l_stkWorkStack.push(r_vecElements[i]);
				//else if we have an operator, 
				else if ((r_vecElements[i].type == BINARY_OPERATOR) || (r_vecElements[i].type == UNARY_OPERATOR)) {
					if (l_stkWorkStack.empty()) {
						l_stkWorkStack.push(r_vecElements[i]);
					}
					//If the precedence of the scanned operator is greater than the precedence of the operator in the stack(or the stack is empty), push it
					else if (l_stkWorkStack.top().precedence <= r_vecElements[i].precedence)
	                //else if (l_stkWorkStack.top().precedence < r_vecElements[i].precedence)
	                //
						l_stkWorkStack.push(r_vecElements[i]);
					else {
						// Else, Pop the operator from the stack until the precedence of the scanned operator is less-equal to the precedence of the operator residing on the top of the stack. Push the scanned operator to the stack.
						while ((!l_stkWorkStack.empty()) && (r_vecElements[i].precedence <= l_stkWorkStack.top().precedence) && (l_stkWorkStack.top().type != OPEN_PARENDS)) {
							l_queElements.push(l_stkWorkStack.top());
							l_stkWorkStack.pop();
						}
						l_stkWorkStack.push(r_vecElements[i]);
					}
				}
				else if (r_vecElements[i].type == CLOSE_PARENDS) {
					//If the scanned character is an ‘)’, pop and output from the stack until an ‘(‘ is encountered.
					while ((!l_stkWorkStack.empty()) && (l_stkWorkStack.top().type != OPEN_PARENDS)) {
						l_queElements.push(l_stkWorkStack.top());
						l_stkWorkStack.pop();
					}
					l_stkWorkStack.pop(); //get rid of open parend
				}
				
			}
		}

		//empty the work stack, add it to the queue.
		while (!l_stkWorkStack.empty()) {
			l_queElements.push(l_stkWorkStack.top());
			l_stkWorkStack.pop();
		}

		return l_queElements;

}

//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
int Expression::computeBinaryOperation(int v_intValue1, int v_intValue2, std::string v_strOperator)
{
	std::string  l_strErrorMessage = "";

	//perform the appropriate binary operation
	if (v_strOperator == "^") {
		return (pow(v_intValue2, v_intValue1));
		//return (v_intValue2 ^ v_intValue1);
	}
	else if (v_strOperator == "*") {
		return (v_intValue2 * v_intValue1);
	}
	else if (v_strOperator == "/") {
		if (v_intValue1 == 0) {
			l_strErrorMessage = "Division by Zero (computeBinaryOperation).";
			throw l_strErrorMessage;
		}
		return (v_intValue2 / v_intValue1);
	}
	else if (v_strOperator == "%") {
		if (v_intValue1 == 0) {
			l_strErrorMessage = "Modula division by Zero (computeBinaryOperation).";
			throw l_strErrorMessage;
		}

		return (v_intValue2 % v_intValue1);
	}
	else if (v_strOperator == "+") {
		return (v_intValue2 + v_intValue1);
	}
	else if (v_strOperator == "-") {
		return (v_intValue2 - v_intValue1);
	}
	else if (v_strOperator == "||") {
		return (v_intValue2 || v_intValue1);
	}
	else if (v_strOperator == "&&") {
		return (v_intValue2 && v_intValue1);
	}
	else if (v_strOperator == "==") {
		return (v_intValue2 == v_intValue1);
	}
	else if (v_strOperator == "!=") {
		return (v_intValue2 != v_intValue1);
	}
	else if (v_strOperator == ">") {
		return (v_intValue2 > v_intValue1);
	}
	else if (v_strOperator == "<") {
		return (v_intValue2 < v_intValue1);
	}
	else if (v_strOperator == ">=") {
		return (v_intValue2 >= v_intValue1);
	}
	else if (v_strOperator == "<=") {
		return (v_intValue2 <= v_intValue1);
	}
	else {
		//This logically should not happen
		l_strErrorMessage = "Invalid element in expression (computeBinaryOperation).";
		throw l_strErrorMessage;
		return 0;
	}

}

//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
int Expression::computeUnaryOperation(int v_intValue1, std::string v_strOperator)
{
	//perform the appropriate unary operation
	if (v_strOperator == "!") {
		return (!v_intValue1);
	}
	else if (v_strOperator == "++") {
		return (++v_intValue1);
	}
	else if (v_strOperator == "--") {
		return (--v_intValue1);
	}
	else {
		//This logically should not happen
		throw "Invalid element in expression (computeUnaryOperation).";
		return 0;
	}
	
}

//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
int Expression::computeResults(std::queue<Element> &r_queElements)
{
	std::stack<int>  l_stkWorkStack;   // work stack of int values to compute mathmatical expression
	int l_intValue1 = 0;               // first int value pulled from stack when we compute sub portion of expression
	int l_intValue2 = 0;               // second int value pulled from stack when we compute sub portion of expression

	//while we still have items in the expression to process.
	while (r_queElements.size() > 0) {
		//based upon which type of element we have, perform the appropriate action.
		switch (r_queElements.front().type)
		{
		// if we get a number, add it to the work stack
		case OPERAND:
			l_intValue1 = std::stoi(r_queElements.front().item);
			l_stkWorkStack.push(l_intValue1);
			break;
		// we we get a binary operator, pop the first two numbers, evaluate the numbers and the operator, push the results back on the stack
		case BINARY_OPERATOR:
			l_intValue1 = l_stkWorkStack.top();
			l_stkWorkStack.pop();
			l_intValue2 = l_stkWorkStack.top();
			l_stkWorkStack.pop();
			l_stkWorkStack.push(computeBinaryOperation(l_intValue1, l_intValue2, r_queElements.front().item));
			break;
		// we we get a unary operator, pop the first number, evaluate the number and the operator, push the results back on the stack
		case UNARY_OPERATOR:
			l_intValue1 = l_stkWorkStack.top();
			l_stkWorkStack.pop();
			l_stkWorkStack.push(computeUnaryOperation(l_intValue1,  r_queElements.front().item));
			break;

		default:
			//This logically should not happen
			throw "Invalid element in expression.";
			break;
		}
		//we processed this element, remove it from the queue
		r_queElements.pop();

	}

	//the answer we are looking should be at the top of the stack
	l_intValue1 = l_stkWorkStack.top();
	l_stkWorkStack.pop();

	//if the stack is now empty, there was no logical error
	if (l_stkWorkStack.empty())
		return l_intValue1;
	else {
		//This logically should not happen
		throw "Not expecting more elements in the Stack. (computeResults)";
		return 0;
	}

		
	
}

//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
Expression::Expression()
{

}

//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
Expression::~Expression()
{
}

//############################################################################################################################
// clear
//
// Purpose:  
//
// Inputs: No inputs
//
// Output: 
//
//############################################################################################################################
int Expression::evaluate(std::string v_strExpression)
{
	int l_intResults = 0;
	std::queue<Element>  l_quePostfixExpression;      //expression represented in postfix notation, broken down by elements
	std::vector<Element> l_vecInfixExpression;        //expression represented in infix notation, broken down by elements
	
	//break up the string into individual elements store them in an
	l_vecInfixExpression = parse(v_strExpression);

	//Check to see if we have any invalid elements, such as a decimal point, 
	//and if the order of the elements is correct, such as not having two operands in a row.
	if (validate(l_vecInfixExpression)) {
		//we have valid data, we will now convert it from an infix expression to a postfix expression so we can evaluate it.
		l_quePostfixExpression = convertToPostfix(l_vecInfixExpression);
		//we will now evaluate the postfix expression
		l_intResults = computeResults(l_quePostfixExpression);

		//temp code to see what  happend
		std::cout << "-------------------" << std::endl;
		std::cout << v_strExpression << std::endl;
		std::cout << l_intResults << std::endl;
		return l_intResults;
	}
	else {
		return 0;
		//raise error
	}


	
}
