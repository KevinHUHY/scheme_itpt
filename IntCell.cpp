#include "IntCell.hpp"

IntCell::IntCell(int i)
:int_m(i)
{}

IntCell::IntCell(double d)
:int_m(static_cast<int>(d))
{}

IntCell::~IntCell()
{}

int IntCell::get_int() const
{
  return int_m;
}

void IntCell::print(std::ostream& os) const
{
  os << int_m;
}

Cell::Type IntCell::get_type() const
{
  return INT;
}
