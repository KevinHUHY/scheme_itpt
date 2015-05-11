#ifndef __arith__
#define __arith__

#include <string>
#include <stdexcept>

class Arith {
public:
  Arith(double d, std::string s) :val(d), symbol(s) {}
  virtual void operator()(double x) { return; };
  double val;
  std::string symbol;
};

class Add : public Arith {
public:
	Add(double d = 0) :Arith(d, "+") {}
	void operator()(double x) { val += x; }
};

class Multiply : public Arith {
public:
	Multiply(double d = 1) :Arith(d, "*") {}
	void operator()(double x) { val *= x; }
};

class Subtraction : public Arith {
public:
  Subtraction(double d = 0) :Arith(d, "-") {}
  void operator()(double x) { val -= x; }
};

class Division : public Arith {
public:
  Division(double d = 1) : Arith(d, "/") {}
  void operator()(double x) { val /= x; }
};

template<class Func>
double get_number(Cell* op, bool* ret_double,const Func& f)
{
  if (op == nil) {
    throw(std::runtime_error("non-numerical input in " + f.symbol));
  }
  if (op->is_int()) {
    return op -> get_int();
  } else if (op->is_double()) {
    *ret_double = true;
    return op -> get_double();
  } else {
    throw(std::runtime_error("non-numerical input in " + f.symbol));
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

template<class Func>
Cell* eval_subt_divi(Cell* args, Environment* env)
{
  Func f;
  if (args == nil) {
    throw(std::runtime_error(f.symbol + " needs at least 1 input"));
  }
	Cell* head = eval(args->get_car(), env);
	Cell* rest = args -> get_cdr();
	if (head == nil) {
		throw(std::runtime_error("non-numerical input in " + f.symbol));
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

#endif /* defined(__arith__) */
