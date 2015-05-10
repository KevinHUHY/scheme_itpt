#include "eval.hpp"
#include "eval_helper.hpp"

using namespace std;

// list < bstmap< string, Cell*> > sym_table_list;
//map<string, Cell*> symbol_table;

bool is_builtin_func(string s)
{
	return s == "ceiling" || s == "floor"  	|| s == "if")
			|| s == "quote"		|| s == "cons"		|| s == "car"
			|| s == "cdr" 		|| s == "car"			|| s == "nullp"
			|| s == "listp" 	|| s == "+" 			|| s == "-"
			|| s == "*"				|| s == "/" 			|| s == "define"
			|| s == "<" 			|| s == "not" 		|| s == "eval"
			|| s == "print" 	|| s == "lambda" 	|| s == "apply";
}

// throw exception when ...
Cell* eval(Cell* const c, Environment* globe_env)
{
	if(c == nil) {
		throw(runtime_error("Empty list cannot be evaluated."));
	}
	if(!listp(c)) {
		if(symbolp(c)) {
			string sym = c -> get_symbol();
			Cell* val = retrieve_symbol(sym, global_env);
			if (val == nil) {
				if (is_builtin_func(sym)) {
					return ProcedureCell(nil, nil);
				} else {
					throw(runtime_error("Undefined symbol"));
				}
			} else {
				return val;
			}
		} else {
			return c;
		}
	} else {
		Cell* func = eval(c -> get_car(), global_env);
		return func -> apply(c -> get_cdr(), global_env);
	}

}
