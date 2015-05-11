#ifndef __BOOLCELL__
#define __BOOLCELL__

#include "Cell.hpp"

class BoolCell : public Cell {
public:
  BoolCell(bool b);
  BoolCell(int i);
  BoolCell(double d);
  virtual ~BoolCell();

  virtual int get_bool() const;
  virtual void print(std::ostream& os = std::cout) const;

private:
  bool bool_m;
  virtual Type get_type() const;
}


#endif /* defined(__BOOLCELL__) */
