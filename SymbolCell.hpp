#ifndef __SymbolCell__
#define __SymbolCell__

#include "Cell.hpp"

class SymbolCell : public Cell {
public:
  SymbolCell(const char* s);
  virtual ~SymbolCell();

  virtual std::string get_symbol() const;
  virtual Cell* apply(Cell* args, Environment* env);

  virtual void print(std::ostream& os = std::cout) const;

private:
  std::string symbol_m;
  Type get_type() const;
};

#endif /* defined(__SymbolCell__)*/
