#include "ProcedureCell.hpp"
#include "eval_helper.hpp"
using std::string;

ProcedureCell::ProcedureCell(Cell* my_formals, Cell* my_body,
														 const Environment* curt_env)
:formals_m(my_formals), body_m(my_body)
{
	if (curt_env != 0) {
		env = (*curt_env);
	}
}

ProcedureCell::~ProcedureCell()
{
	if(formals_m != nil) {
		delete formals_m;
	}
	if(body_m != nil) {
		delete body_m;
	}
}

Cell* ProcedureCell::get_formals() const
{
	return formals_m;
}

Cell* ProcedureCell::get_body() const
{
	return body_m;
}

Environment& ProcedureCell::get_environment()
{
	return env;
}

std::string ProcedureCell::get_name() const
{
	return name;
}

void ProcedureCell::set_name(string s)
{
	name = s;
}

Cell* ProcedureCell::apply(Cell* args, Environment* env)
{
	return eval_procedure(this, args, env);
}

void ProcedureCell::print(std::ostream& os) const
{
	os << "#<function>";
}

Cell::Type ProcedureCell::get_type() const
{
	return PROCEDURE;
}
