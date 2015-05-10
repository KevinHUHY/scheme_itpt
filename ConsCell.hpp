#ifndef __ConsCell__
#define __ConsCell__

#include "Cell.hpp"

class ConsCell : public Cell {
public:
  ConsCell(Cell* my_car, Cell* my_cdr);
  virtual ~ConsCell();

  virtual Cell* get_car() const;
  virtual Cell* get_cdr() const;

  virtual void print(std::ostream& os = std::cout) const;

private:
  virtual Type get_type() const;
  Cell* car_m;
  Cell* cdr_m;
};

#endif /* defined(__ConsCell__) */
