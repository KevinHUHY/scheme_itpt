#include "Cell.hpp"
using namespace std;

// here is the logical place to define nil
Cell* const nil = 0;

string type2string(Cell::Type t)
{
  if (t == Cell::INT) {
    return string("Int");
  } else if (t == Cell::DOUBLE) {
    return string("Double");
  } else if (t == Cell::SYMBOL) {
    return string("Symbol");
  } else if (t == Cell::CONS) {
    return string("Cons");
  } else {
    return string("Procedure");
  }
}

bool Cell::is_int() const
{
  return get_type() == INT;
}

bool Cell::is_double() const
{
  return get_type() == DOUBLE;
}

bool Cell::is_symbol() const
{
  return get_type() == SYMBOL;
}

bool Cell::is_cons() const
{
  return get_type() == CONS;
}

bool Cell::is_procedure() const
{
  return get_type() == PROCEDURE;
}

int Cell::get_int() const
{
  string err_msg = "Cannot get integer from "
                  + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

double Cell::get_double() const
{
  string err_msg = "Cannot get double from "
                  + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

string Cell::get_symbol() const
{
  string err_msg = "Cannot get a symbol from "
                  + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

Cell* Cell::get_car() const
{
  string err_msg = "Cannot get car from "
                  + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

Cell* Cell::get_cdr() const
{
  string err_msg = "Cannot get cdr from "
                  + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

Cell* Cell::get_formals() const
{
  string err_msg = "Cannot get formal arg(s) from "
                  + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

Cell* Cell::get_body() const
{
  string err_msg = "Cannot get function body from "
                  + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

Environment& Cell::get_environment()
{
  string err_msg = "Cannot get environment from "
                  + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

string Cell::get_name() const
{
  string err_msg = "Cannot get name for " + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

void Cell::set_name(string s)
{
  string err_msg = "Cannot set name for " + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}

Cell* Cell::apply(Cell* args, Environment* env)
{
  string err_msg = "Cannot apply " + type2string(get_type()) + "cell.";
  throw(runtime_error(err_msg));
}
