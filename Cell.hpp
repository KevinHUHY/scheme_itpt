#ifndef __CELL__
#define __CELL__

#include <cstdlib>
#include <iostream>
#include <string>
#include <list>
#include <map>

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
  // C++ does not allow for virtual constructors.
  // A virtual destructor is necessary if derived class has a non-trivial destrcutor
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

  virtual Cell* apply(Cell* const args, Environment* env);

  virtual void print(std::ostream& os = std::cout) const = 0;

  friend std::string type2string(Cell::Type t);

  //TODO: get rid of these two functions
  bool is_number() const;
  double get_number() const;
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
