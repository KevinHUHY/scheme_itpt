#include "arith.hpp"
#include "eval_helper.hpp"
using namespace std;

template<class Func>
double get_number(Cell* op, bool* ret_double,const Func& f)
{
  if (op == nil) {
    throw(runtime_error("non-numerical input in " + f.symbol));
  }
  if (op->is_int()) {
    return op -> get_int();
  } else if (op->is_double()) {
    *ret_double = true;
    return op -> get_double();
  } else {
    throw(runtime_error("non-numerical input in " + f.symbol));
  }
}

template<class Func>
bool loop_over_ops(Func& f, Cell* args, Environment* env)
{
	bool ret_double = false;
	while (args != nil) {
		Cell* op = eval(args->get_car(), env);
		double op_num = get_number(op, &ret_double, f);
    f(op_num);
		args = args->get_cdr();
	}
	return ret_double;
}

template<class Func>
Cell* eval_plus_mult(Cell* args, Environment* env)
{
	Func f;
	bool ret_double = loop_over_ops(f, args, env);
	if (ret_double) {
		return new DoubleCell(f.val);
	} else {
		return new IntCell(f.val);
	}
}

Cell* eval_plus(Cell* args, Environment* env)
{
	return eval_plus_mult<Add>(args, env);
}

Cell* eval_mult(Cell* args, Environment* env)
{
	return eval_plus_mult<Multiply>(args, env);
}

template<class Func>
Cell* eval_subt_divi(Cell* args, Environment* env)
{
  Func f;
  if (args == nil) {
    throw(runtime_error(f.symbol + " needs at least 1 input"));
  }
	Cell* head = eval(args->get_car(), env);
	Cell* rest = args -> get_cdr();
	if (head == nil) {
		throw(runtime_error("non-numerical input in " + f.symbol));
	}

	bool ret_double = false;
	double head_num = get_number(head, &ret_double, f);
	if (rest == nil) {
		f(head_num);
		if (ret_double) {
			return new DoubleCell(f.val);
		} else {
			return new IntCell(f.val);
		}
	} else {
    f.val = head_num;
		ret_double = (ret_double || loop_over_ops(f, rest, env));
		if (ret_double) {
			return new DoubleCell(f.val);
		} else {
			return new IntCell(f.val);
		}
	}
}

Cell* eval_subt(Cell* args, Environment* env)
{
  return eval_subt_divi<Subtraction>(args, env);
}

Cell* eval_divi(Cell* args, Environment* env)
{
  return eval_subt_divi<Division>(args, env);
}
