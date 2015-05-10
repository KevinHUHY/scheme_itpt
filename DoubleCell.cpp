#include "DoubleCell.hpp"

DoubleCell::DoubleCell(double d)
:double_m(d)
{}

DoubleCell::~DoubleCell()
{}

double DoubleCell::get_double() const
{
  return double_m;
}

void DoubleCell::print(std::ostream& os) const
{
  os << double_m;
  if((double_m - (int)double_m) < 0.000001) {
    os << ".0";
  }
}

Cell::Type DoubleCell::get_type() const
{
  return DOUBLE;
}
