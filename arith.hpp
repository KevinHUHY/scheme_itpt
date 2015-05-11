#ifndef __arith__
#define __arith__

#include <string>

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

#endif /* defined(__arith__) */
