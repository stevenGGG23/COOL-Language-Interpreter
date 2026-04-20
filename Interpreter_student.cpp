//Steven Gobran 
//Project 7  
//Interpreter_student.cpp

#include <sstream>
#include <list>
#include <algorithm>
#include <map>

#include "Absyn.h"
#include "Semant.h"
#include "lib.h"

using namespace std;
using namespace absyn;


///////////////////////////////////////////////////////////////////////////////
//
//  Interpreter Features
//
//  For each class of expression, there is an interpreter method to evaluate it.
//  The interpreter methods make use of the environments previously constructed
//  for each class.  This code parallels the structure of the type
//  inference rules in the CoolAid very closely.
//
///////////////////////////////////////////////////////////////////////////////

ValueP IntExp::interp_student(EnvironmentP env)
{
	return new ValueEntry(stoi(getValue()->getContent()));
}

ValueP BoolExp::interp_student(EnvironmentP env)
{
	return new ValueEntry(getValue());
}

ValueP StringExp::interp_student(EnvironmentP)
{
	return new ValueEntry(getValue()->getContent());
}

ValueP OpExp::interp_student(EnvironmentP env)
{
	// Evaluate both operands first
	ValueP t1 = left->interp(env);
	ValueP t2 = right->interp(env);

	switch (op) {
		case PLUS:
			return new ValueEntry(t1->getIntValue() + t2->getIntValue());
		case MINUS:
			return new ValueEntry(t1->getIntValue() - t2->getIntValue());
		case MUL:
			return new ValueEntry(t1->getIntValue() * t2->getIntValue());
		case DIV:
			return new ValueEntry(t1->getIntValue() / t2->getIntValue());
		case LT:
			return new ValueEntry(t1->getIntValue() < t2->getIntValue());
		case LE:
			return new ValueEntry(t1->getIntValue() <= t2->getIntValue());
		case EQ:
			if (t1->isINT())
				return new ValueEntry(t1->getIntValue() == t2->getIntValue());
			else if (t1->isSTRING())
				return new ValueEntry(t1->getStrValue() == t2->getStrValue());
			else
				// Object equality: compare by identity (pointer to environment)
				return new ValueEntry(t1->getObjectValue() == t2->getObjectValue());
	}

	// Should never reach here
	return new ValueEntry();
}

ValueP NotExp::interp_student(EnvironmentP env)
{
	// Evaluate the sub-expression, then negate its boolean value
	ValueP v = expr->interp(env);
	return new ValueEntry(!v->getBoolValue());
}

ValueP ObjectExp::interp_student(EnvironmentP env)
{
	// "self" is a special keyword: return a value representing the current object
	if (name == self) {
		return new ValueEntry(env->get_self_type(), env);
	}
	// Otherwise look up the variable's current value in the environment
	return env->var_value_lookup(name);
}

ValueP NewExp::interp_student(EnvironmentP env)
{
	// Look up the class in the inheritance hierarchy
	InheritanceNodeP node = env->lookup_class(type_name);

	// Instantiate a fresh environment (object) for that class
	EnvironmentP obj_env = node->instantiation();

	// Return a value holding the object's type and its environment
	return new ValueEntry(type_name, obj_env);
}

ValueP IsvoidExp::interp_student(EnvironmentP env)
{
	// Evaluate the sub-expression and check whether the result is VOID
	ValueP v = expr->interp(env);
	return new ValueEntry(v->isVOID());
}

ValueP LetExp::interp_student(EnvironmentP env)
{
	// Open a new scope for the let-bound variable
	env->var_enterScope();

	// Declare the variable with its declared type (default value is VOID)
	env->var_add(identifier, type_decl);

	// If an initializer expression is present (and not a NoExp placeholder),
	// evaluate it and store the result in the newly declared variable
	if (init != nullptr && dynamic_cast<NoExp*>(init) == nullptr) {
		ValueP v = init->interp(env);
		env->var_value_change(identifier, v);
	}

	// Evaluate the body with the let variable in scope
	ValueP v = body->interp(env);

	// Restore the previous scope
	env->var_exitScope();

	return v;
}

ValueP BlockExp::interp_student(EnvironmentP env)
{
	// Evaluate each expression in the block in order;
	// the value of the block is the value of the last expression
	ValueP v = new ValueEntry(); // VOID default (in case body is somehow empty)
	const List<Expression>* cur = body;
	while (cur != nullptr) {
		v = cur->getHead()->interp(env);
		cur = cur->getRest();
	}
	return v;
}

ValueP AssignExp::interp_student(EnvironmentP env)
{
	// Evaluate the right-hand side
	ValueP v = expr->interp(env);

	// Update the variable's value in the environment
	env->var_value_change(name, v);

	// The value of an assignment is the value that was assigned
	return env->var_value_lookup(name);
}

ValueP IfExp::interp_student(EnvironmentP env)
{
	// Evaluate the predicate
	ValueP cond = pred->interp(env);

	// Branch on the boolean result
	if (cond->getBoolValue()) {
		return then_exp->interp(env);
	} else {
		return else_exp->interp(env);
	}
}

ValueP WhileExp::interp_student(EnvironmentP env)
{
	// Evaluate the predicate to check the loop condition
	ValueP cond = pred->interp(env);

	while (cond->getBoolValue()) {
		// Execute the loop body
		body->interp(env);

		// Re-evaluate the predicate for the next iteration
		cond = pred->interp(env);
	}

	// A while loop always returns VOID
	return new ValueEntry();
}

ValueP CallExp::interp_student(EnvironmentP env)
{
	//No need to implement this one.
	//Teacher's solution will be used.
	return new ValueEntry();
}

ValueP StaticCallExp::interp_student(EnvironmentP env)
{
	//No need to implement this one.
	//Teacher's solution will be used.
	return new ValueEntry();
}

ValueP Branch_class::interp_student(EnvironmentP env)
{
	//No need to implement this one.
	//Teacher's solution will be used.
	return new ValueEntry();
}

ValueP CaseExp::interp_student(EnvironmentP env)
{
	//No need to implement this one.
	//Teacher's solution will be used.
	return new ValueEntry();
}

ValueP NoExp::interp_student(EnvironmentP)
{
	//No need to implement this one.
	//Teacher's solution will be used.
	return new ValueEntry();
}