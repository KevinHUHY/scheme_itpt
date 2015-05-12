#include "eval.hpp"
#include "eval_helper.hpp"

using namespace std;

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
		Cell* val = retrieve_symbol(c, global_env);
		return val;
	} else {
		return c;
	}
}
