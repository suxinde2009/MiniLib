//
//  mtlExpression.cpp
//  MiniLib
//
//  Created by Jonathan Karlsson on 10/14/14.
//  Copyright (c) 2014 Jonathan Karlsson. All rights reserved.
//

#include "mtlExpression.h"
#include <cmath>

float mtlExpression::OperationNode::Evaluate( void ) const
{
	float lval = left->Evaluate();
	float rval = right->Evaluate();
	float result;
	
	switch (operation) {
		case '+':
			result = lval + rval;
			break;
		case '-':
			result = lval - rval;
			break;
		case '*':
			result = lval * rval;
			break;
		case '/':
			result = lval / rval;
			break;
		case '^':
			result = pow(lval, rval);
			break;
	}
	
	return result;
}

float mtlExpression::ValueNode::Evaluate( void ) const
{
	return value;
}

mtlExpression::mtlExpression( void ) : m_expression(), m_result(0.0f), m_root(NULL)
{}

mtlExpression::~mtlExpression( void )
{
	DestroyTermTree(m_root);
}

bool mtlExpression::IsTermBalanced( void ) const
{
	// check so that each term has an operation and two operands
	return true;
}

bool mtlExpression::IsBraceBalanced( void ) const
{
	int stack = 0;
	for (int i = 0; i < m_expression.GetSize(); ++i) {
		char ch = m_expression.GetChars()[i];
		if (ch == '(') {
			++stack;
		} else if (ch == ')') {
			if (stack > 0) {
				--stack;
			} else {
				return false;
			}
		}
	}
	return stack == 0;
}

void mtlExpression::SanitizeExpression( void )
{
	int skipped = 0;
	for (int i = 0; i < m_expression.GetSize(); ++i) {
		char ch = m_expression.GetChars()[i];
		if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
			++skipped;
		} else {
			m_expression.GetChars()[i - skipped] = ch;
		}
	}
	m_expression.SetSize(m_expression.GetSize() - skipped);
}

void mtlExpression::DestroyTermTree(mtlExpression::TermNode *node)
{
	if (node == NULL) { return; }
	DestroyTermTree(node->left);
	DestroyTermTree(node->right);
	delete node;
}

void mtlExpression::GenerateTermTree(mtlExpression::TermNode *& node, const mtlChars &expression)
{
	if (expression.GetSize() == 0) {
		node = NULL;
		return;
	}
	
	char operations[] = {
		'+', '-', '*', '/', '^'
	};
	
	int opIndex = -1;
	char op;
	
	for (int i = 0; i < sizeof(operations); ++i) {
		opIndex = FindOperation(operations[i], expression);
		if (opIndex != -1) {
			op = operations[i];
			break;
		}
	}
	
	if (opIndex != -1) {
		
		OperationNode *opNode = new OperationNode;
		opNode->operation = op;
		opNode->left = NULL;
		opNode->right = NULL;
		
		mtlChars lexpr = mtlChars(expression, 0, opIndex);
		mtlChars rexpr = mtlChars(expression, opIndex + 1, expression.GetSize());
		
		GenerateTermTree(opNode->left, lexpr);
		GenerateTermTree(opNode->right, rexpr);

		node = opNode;
	} else if (expression.GetChars()[0] == '(' && expression.GetChars()[expression.GetSize() - 1] == ')') {
		
		GenerateTermTree(node, mtlChars(expression, 1, expression.GetSize() - 1));
		
	} else { // STOPPING CONDITION
		
		ValueNode *valNode = new ValueNode;
		valNode->left = NULL;
		valNode->right = NULL;
		valNode->value = 0.0f;
		if (!expression.ToFloat(valNode->value)) {
			valNode->value = GetConstant(expression);
		}
		node = valNode;
	
	}
}

int mtlExpression::FindOperation(char operation, const mtlChars &expression) const
{
	int braceStack = 0;
	for (int i = 0; i < expression.GetSize(); ++i) {
		char ch = expression.GetChars()[i];
		if (ch == '(') {
			++braceStack;
		} else if (ch == ')') {
			--braceStack;
		} else if (braceStack == 0 && ch == operation) { // contents of parenthesis are not parsed
			return i;
		}
	}
	return -1;
}

void mtlExpression::SetConstant(const mtlChars &name, float value)
{
	if (name.GetSize() == 0) {
		return;
	}
	for (int i = 1; i < name.GetSize(); ++i) {
		char ch = name.GetChars()[i];
		if (!mtlChars::IsAlphanumeric(ch) && ch != '_') { return; }
	}
	char ch = name.GetChars()[0];
	if (!mtlChars::IsAlpha(ch) && ch != '_') {
		return;
	}
	(*m_constants.CreateEntry(name)) = value;
}

float mtlExpression::GetConstant(const mtlChars &name) const
{
	const float *value = m_constants.GetEntry(name);
	return value != NULL ? *value : 0.0f;
}

bool mtlExpression::SetExpression(const mtlChars &expression)
{
	m_expression.Copy(expression);
	SanitizeExpression();
	bool retVal = IsBraceBalanced() && IsTermBalanced();
	if (!retVal) {
		m_expression.Free();
	}
	DestroyTermTree(m_root);
	GenerateTermTree(m_root, m_expression);
	return retVal;
}

const mtlString &mtlExpression::GetExpression( void ) const
{
	return m_expression;
}

float mtlExpression::Evaluate( void ) const
{
	float result = 0.0f;
	if (m_root != NULL) {
		result = m_root->Evaluate();
	}
	return result;
}
