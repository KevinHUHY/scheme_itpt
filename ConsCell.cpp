#include "ConsCell.hpp"

ConsCell::ConsCell(Cell* my_car, Cell* my_cdr)
:car_m(my_car), cdr_m(my_cdr)
{}

ConsCell::~ConsCell()
{
	if(car_m != nil) {
    delete car_m;
  }
	if(cdr_m != nil) {
    delete cdr_m;
  }
}

Cell* ConsCell::get_car() const
{
	return car_m;
}

Cell* ConsCell::get_cdr() const
{
	return cdr_m;
}

void ConsCell::print(std::ostream& os) const
{
  os << "(";
  const Cell* current = this;
  while(current != nil) {
    if(current != this) {
      os << " ";
    }
    if(current -> get_car() != nil) {
      current -> get_car() -> print();
    }
    if(current -> get_car() == nil) {
      os << "()";
    }
    current = current -> get_cdr();
    if(current != nil && !current -> is_cons()) {
      os << " .";
      current -> print();
      current = nil;
    }
  }
  os << ")";
}

Cell::Type ConsCell::get_type() const
{
  return CONS;
}
