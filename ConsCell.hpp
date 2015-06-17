#ifndef __ConsCell__
#define __ConsCell__

#include "Cell.hpp"

class ConsCell : public Cell {
public:
  ConsCell(Cell* my_car, Cell* my_cdr, int level);
  virtual ~ConsCell();

  virtual Cell* get_car() const;
  virtual Cell* get_cdr() const;
  virtual int get_level() const;
  virtual void set_level(int n);
  virtual void print(std::ostream& os = std::cout) const;

private:
  virtual Type get_type() const;
  Cell* car_m;
  Cell* cdr_m;
  int level_m;
};

#endif /* defined(__ConsCell__) */
