#include "SymbolCell.hpp"
#include "eval_helper.hpp"

// using std::string;
using namespace std;
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
  if(op == "ceiling") {
    return eval_ceiling(args, env);
  } else if (op == "floor") {
    return eval_floor(args, env);
  } else if (op == "if") {
    return eval_if(args, env);
  } else if (op == "quote") {
    return eval_quote(args);
  } else if (op == "cons") {
    return eval_cons(args, env);
  } else if (op == "car") {
    return eval_car(args, env);
  } else if (op == "cdr") {
    return eval_cdr(args, env);
  } else if (op == "nullp") {
    return eval_nullp(args, env);
  } else if (op == "listp") {
    return eval_listp(args, env);
  } else if (op == "define") {
    return eval_define(args, env);
  } else if (op == "<") {
    return eval_lessthan(args, env);
  } else if (op == "not") {
    return eval_not(args, env);
  } else if (op == "eval") {
    return eval_eval(args, env);
  } else if (op == "print") {
    return eval_print(args, env);
  } else if (op == "lambda") {
    return eval_lambda(args, env);
  } else if (op == "apply") {
    return eval_apply(args, env);
  } else if (op == "+") {
    return eval_plus(args, env);
  } else if (op == "*") {
    return eval_mult(args, env);
  } else if (op == "-") {
    return eval_subt(args, env);
  } else if (op == "/") {
    return eval_divi(args, env);
  } else if (op == "let") {
    return eval_let(args, env);
  } else {
    throw(runtime_error("cannot apply " + op));
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
