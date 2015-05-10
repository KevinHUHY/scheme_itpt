#ifndef __ProcedureCell__
#define __ProcedureCell__

#include "Cell.hpp"

class ProcedureCell : public Cell {
public:
  ProcedureCell(Cell* my_formals, Cell* my_body, const Environment* curt_env);
  virtual ~ProcedureCell();

  virtual Cell* get_formals() const;
  virtual Cell* get_body() const;
  virtual Cell* apply(Cell* const args);

  virtual void print(std::ostream& os = std::cout) const;

private:
  Type get_type() const;
  Cell* formals_m;
  Cell* body_m;
  Environment env;
};

#endif /* defined(__ProcedureCell) */
