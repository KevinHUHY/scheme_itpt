#include "ProcedureCell.hpp"
#include "eval_helper.hpp"

ProcedureCell::ProcedureCell(Cell* my_formals, Cell* my_body,
														 const Environment* curt_env)
:formals_m(my_formals), body_m(my_body)
{
	env = (*curt_env);
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

Cell* ProcedureCell::apply(Cell* args)
{
	return deal_procedure(this, args);
}

void ProcedureCell::print(std::ostream& os) const
{
	os << "#<function>";
}

Cell::Type ProcedureCell::get_type() const
{
	return PROCEDURE;
}
