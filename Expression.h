#pragma once

#include <vector>
#include <stack>
#include <queue>
#include <string>

const int  INVALID_CHAR = -1;
const int  OPERAND = 1;
const int  BINARY_OPERATOR = 2;
const int  UNARY_OPERATOR = 3;
const int  OPEN_PARENDS = 4;
const int  CLOSE_PARENDS = 5;

struct Element
{
	std::string item;
	unsigned int position;
	int type;
	int precedence;
};

class Expression
{
private:
	//std::string          strErrorMessage;

	Element getOperand(std::string v_strExpression, bool &r_bolNegative, unsigned int v_untSize, int v_intNegativeSignPosition, unsigned int &r_untIndex);
	std::vector<Element> parse(std::string v_strExpression);
	bool validate(std::vector<Element> &r_vecElements);
	std::queue<Element> convertToPostfix(std::vector<Element> &r_vecElements);
	int  computeBinaryOperation(int v_intValue1, int v_intValue2, std::string v_strOperator);
	int  computeUnaryOperation(int v_intValue1, std::string v_strOperator);
	int  computeResults(std::queue<Element> &r_queElements);

public:
	Expression();
	~Expression();

	int  evaluate(std::string  v_strExpression);
};

