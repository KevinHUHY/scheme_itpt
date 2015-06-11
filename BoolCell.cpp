#include "BoolCell.hpp"

BoolCell::BoolCell(bool b)
  : bool_m(b)
{}

BoolCell::~BoolCell()
{}

bool BoolCell::get_bool() const
{
  return bool_m;
}

void BoolCell::print(std::ostream& os) const
{
  if (bool_m) {
    os << "#t";
  } else {
    os << "#f";
  }
}

Cell::Type BoolCell::get_type() const
{
  return BOOL;
}
