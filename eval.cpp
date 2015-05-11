#include "eval.hpp"
#include "eval_helper.hpp"

using namespace std;

bool is_builtin_func(string s)
{
	return s == "ceiling" || s == "floor"  	|| s == "if"
			|| s == "quote"		|| s == "cons"		|| s == "car"
			|| s == "cdr" 		|| s == "car"			|| s == "nullp"
			|| s == "listp" 	|| s == "+" 			|| s == "-"
			|| s == "*"				|| s == "/" 			|| s == "define"
			|| s == "<" 			|| s == "not" 		|| s == "eval"
			|| s == "print" 	|| s == "lambda" 	|| s == "apply";
}

// throw exception when ...
Cell* eval(Cell* c, Environment* global_env)
{
	assert(global_env != 0);
	if (c == nil) {
		throw(runtime_error("Empty list cannot be evaluated."));
	}
	if (c -> is_cons()) {
		Cell* func = eval(c->get_car(), global_env);
		if (func == nil) {
			throw(runtime_error("bad syntax, god knows what you are doing"));
		}
		return func -> apply(c->get_cdr(), global_env);
	} else if (c -> is_symbol()) {
		string sym = c -> get_symbol();
		Cell* val = retrieve_symbol(sym, global_env);
		if (val != nil) {
			return val;
		}
		if (is_builtin_func(sym)) {
			return c;
		}
		throw(runtime_error("undefined symbol: " + sym));
	} else {
		return c;
	}
}
