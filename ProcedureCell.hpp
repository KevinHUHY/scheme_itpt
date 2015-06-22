#ifndef __ProcedureCell__
#define __ProcedureCell__

#include "Cell.hpp"
#include <string>

const std::string ANONYMOUS_FUNC = "__ANONYMOUS__";

class ProcedureCell : public Cell {
public:
  ProcedureCell(Cell* my_formals, Cell* my_body, const Environment* curt_env);
  virtual ~ProcedureCell();

  virtual Cell* get_formals() const;
  virtual Cell* get_body() const;
  virtual Environment& get_environment();
  virtual std::string get_name() const;
  virtual void set_name(std::string name);
  virtual Cell* apply(Cell* args, Environment* env);

  virtual void print(std::ostream& os = std::cout) const;

private:
  Type get_type() const;
  Cell* formals_m;
  Cell* body_m;
  std::string name_m;
  Environment env_m;
};

#endif /* defined(__ProcedureCell) */
