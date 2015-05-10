#include "SymbolCell.hpp"
#include "eval_helper.hpp"

SymbolCell::SymbolCell(const char* s)
:symbol_m(s)
{}

SymbolCell::~SymbolCell()
{}

std::string SymbolCell::get_symbol() const
{
  return symbol_m;
}

Cell* SymbolCell::apply(Cell* const args, Environment* env)
{
  string op = get_symbol();
  if(op == "ceiling" || op == "floor") {
    return deal_ceiling_floor(args, op);
  } else if(op == "if") {
    return deal_if(args);
  } else if(op == "quote") {
    return deal_quote(args);
  } else if(op == "cons") {
    return deal_cons(args);
  } else if(op == "car") {
    return deal_car(args);
  } else if(op == "cdr") {
    return deal_cdr(args);
  } else if(op == "nullp") {
    return deal_nullp(args);
  } else if(op == "listp") {
    return deal_listp(args);
  } else if(op == "+") {
    return deal_plus(args);
  } else if(op == "-") {
    return deal_subt(args);
  } else if(op == "*") {
    return deal_mult(args);
  } else if(op == "/") {
    return deal_divi(args);
  } else if(op == "define") {
    return deal_define(args);
  } else if(op == "<") {
    return deal_lessthan(args);
  } else if(op == "not") {
    return deal_not(args);
  } else if(op == "eval") {
    return deal_eval(args);
  } else if(op == "print") {
    return deal_print(args);
  } else if(op == "lambda") {
    return deal_lambda(args);
  } else if(op == "apply") {
    return deal_apply(args);
  } else {
    throw(runtime_error("cannot apply a value that is not a function"));
    return nil;
  }
}

void SymbolCell::print(std::ostream& os) const
{
  os << symbol_m;
}

Cell::Type SymbolCell::get_type() const
{
  return SYMBOL;
}
