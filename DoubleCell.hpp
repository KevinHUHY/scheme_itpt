#ifndef __DoubleCell__
#define __DoubleCell__

#include "Cell.hpp"

class DoubleCell : public Cell {
public:
  DoubleCell(double i);
  virtual ~DoubleCell();

  virtual double get_double() const;
  virtual void print(std::ostream& os = std::cout) const;

private:
  double double_m;
  virtual Type get_type() const;
};

#endif /* defined(__DoubleCell__) */
