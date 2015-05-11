#ifndef __CELL__
#define __CELL__

#include <cstdlib>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <stdexcept>
#include <fstream>
#include <cassert>

class Cell;

typedef std::map<std::string, Cell*> SymbolTable;
typedef std::list<std::map<std::string, Cell*>> Environment;

// Abtract data type Cell
class Cell {
protected:
  enum Type {INT, DOUBLE, SYMBOL, CONS, PROCEDURE};

private:
  virtual Type get_type() const = 0;

public:
  virtual ~Cell(){};

  bool is_int() const;
  bool is_double() const;
  bool is_symbol() const;
  bool is_cons() const;
  bool is_procedure() const;

  virtual int get_int() const;
  virtual double get_double() const;
  virtual std::string get_symbol() const;
  virtual Cell* get_car() const;
  virtual Cell* get_cdr() const;
  virtual Cell* get_formals() const;
  virtual Cell* get_body() const;
  virtual Environment& get_environment();
  virtual std::string get_name() const;
  virtual void set_name(std::string s);

  virtual Cell* apply(Cell* args, Environment* env);
  virtual void print(std::ostream& os = std::cout) const = 0;

  friend std::string type2string(Cell::Type t);
};

/*
  should be defined once in cpp file, otherwise there will be
  multiple definition if this file is included several times
*/
extern Cell* const nil;

#include "IntCell.hpp"
#include "DoubleCell.hpp"
#include "ConsCell.hpp"
#include "SymbolCell.hpp"
#include "ProcedureCell.hpp"

#endif /* defined(__CELL__)*/
