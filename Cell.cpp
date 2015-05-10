#include "Cell.hpp"
using std::string;

// Reminder: cons.hpp expects nil to be defined somewhere.  For this
// implementation, this is the logical place to define it.
Cell* const nil = 0;

string type2string(Cell::Type t)
{
  if (t == INT) {
    return string("Int");
  } else if (t == DOUBLE) {
    return string("Double");
  } else if (t == SYMBOL) {
    return string("Symbol");
  } else if (t == CONS) {
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
                  + type2string(get_type()) + " cell."
  throw(runtime_error(err_msg));
  return 0;
}

double Cell::get_double() const
{
  string err_msg = "Cannot get double from "
                  + type2string(get_type()) + " cell."
  throw(runtime_error(err_msg));
  return 0.0;
}

string Cell::get_symbol() const
{
  string err_msg = "Cannot get a symbol from "
                  + type2string(get_type()) + " cell."
  throw(runtime_error(err_msg));
  return string("");
}

Cell* Cell::get_car() const
{
  string err_msg = "Cannot get car from "
                  + type2string(get_type()) + " cell."
  throw(runtime_error(err_msg));
  return nil
}

Cell* Cell::get_cdr() const
{
  string err_msg = "Cannot get cdr from "
                  + type2string(get_type()) + " cell."
  throw(runtime_error(err_msg));
  return nil;
}

Cell* Cell::get_formals() const
{
  string err_msg = "Cannot get formal arg(s) from "
                  + type2string(get_type()) + " cell."
  throw(runtime_error(err_msg));
  return nil;
}

Cell* Cell::get_body() const
{
  string err_msg = "Cannot get function body from "
                  + type2string(get_type()) + " cell."
  throw(runtime_error(err_msg));
  return nil;
}

Cell* Cell::apply(Cell* const args, Environment* env)
{
  string err_msg = "Cannot apply " + type2string(get_type()) + " cell."
  throw(runtime_error(err_msg));
  return nil;
}

bool Cell::is_number() const
{
  return is_int() || is_double();
}

double Cell::get_number() const
{
  if(is_int()) {
    return get_int();
  } else if(is_double()) {
    return get_double();
  } else {
    string err_msg = "Try to get a number from ";
    if(is_symbol()) {
      err_msg += "a SymbolCell.";
    } else if(is_cons()) {
      err_msg += "a ConsCell.";
    } else {
      err_msg += "Impossible Cell.";
    }
    throw(runtime_error(err_msg));
  }
}
