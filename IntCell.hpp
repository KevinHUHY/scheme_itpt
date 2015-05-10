#ifndef __IntCell__
#define __IntCell__

#include "Cell.hpp"

class IntCell : public Cell {
public:
  IntCell(int i);
  IntCell(double d);
  virtual ~IntCell();

  virtual int get_int() const;
  virtual void print(std::ostream& os = std::cout) const;

private:
  int int_m;
  virtual Type get_type() const;
};

#endif /* defined(__IntCell__) */
