#include "eval_helper.hpp"
#include "arith.hpp"
#include <cmath>
#include <limits>
#include <string>
#include <set>
#include <cassert>
#include <iostream>

using namespace std;

inline bool is_list(Cell* c)
{
	return c == nil || c -> is_cons();
}

int list_length(Cell* c)
{
	int n = 0;
	while(c != nil) {
		c = c -> get_cdr();
		++n;
	}
	return n;
}

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

void check_length(int expected, string op, Cell* c)
{
	int n = list_length(c);
	if(n != expected) {
		string err_msg = op + ": wrong number of actual arguments\n\texpected:"
      + to_string(expected) + "\n\tgiven:" + to_string(n);
		throw(runtime_error(err_msg));
	}
}

Cell* eval_ceiling_floor(Cell* args, string f, Environment* env)
{
	assert(f == "floor" || f == "ceiling");
	check_length(1, f, args);

	Cell* op = args -> get_car();
	op = eval(op, env);
	if (op == nil) {
		throw(runtime_error("No input for " + f));
	}

	double d;
	try{
		d = op -> get_double();
	} catch(runtime_error& e) {
		throw(runtime_error(f + " only accept double as input"));
	}

	if(f == "ceiling") {
		return new IntCell(ceil(d));
	} else {
		return new IntCell(floor(d));
	}
}

Cell* eval_ceiling(Cell* args, Environment* env)
{
	return eval_ceiling_floor(args, "ceiling", env);
}

Cell* eval_floor(Cell* args, Environment* env)
{
	return eval_ceiling_floor(args, "floor", env);
}

// if-then-else, length must be 3
Cell* eval_if(Cell* args, Environment* env)
{
	check_length(3, "if", args);

	Cell* pred = args -> get_car();
	pred = eval(pred, env);
	bool bpred = true;
	if (pred != nil && pred -> is_bool() && pred -> get_bool() == false) {
		bpred = false;
	}

	Cell* ret = nil;
	if (bpred == true) {
		ret = args -> get_cdr() -> get_car();
	} else {
		ret = args -> get_cdr() -> get_cdr() -> get_car();
	}
	return eval(ret, env);
}

//quote never evals anything
Cell* eval_quote(Cell* args)
{
	check_length(1, "quote", args);
	return args -> get_car();
}

void set_list_level(Cell* list, int level)
{
  while (list != nil && list -> is_cons()) {
    list -> set_level(level);
    if (list -> get_car() != nil && list ->get_car() -> is_cons()) {
      set_list_level(list->get_car(), level+1);
    }
    list = list -> get_cdr();
  }
}

//cons constructs conscell, but never sustitute the symbol
//should return correctly on the following input
//(cons 1 (cons (cons 2 nil) nil))
Cell* eval_cons(Cell* args, Environment* env)
{
	check_length(2, "cons", args);
	Cell* op1 = args -> get_car();
	Cell* op2 = args -> get_cdr() -> get_car();
  op1 = eval(op1, env);
  op2 = eval(op2, env);
  Cell* ret = new ConsCell(op1, op2, 1);
  set_list_level(ret, 1);
  return ret;
}

Cell* eval_car_cdr(Cell* args, string f, Environment* env)
{
	assert(f == "car" || f == "cdr");
	check_length(1, f, args);
	Cell* op = args -> get_car();
	op = eval(op, env);
	if (op == nil) {
		throw(runtime_error("Try to get " + f + " from nil"));
	}
	if (f == "car") {
		return op -> get_car();
	} else {
		return op -> get_cdr();
	}
}

Cell* eval_car(Cell* args, Environment* env)
{
	return eval_car_cdr(args, "car", env);
}

Cell* eval_cdr(Cell* args, Environment* env)
{
	return eval_car_cdr(args, "cdr", env);
}

Cell* eval_nullp(Cell* args, Environment* env)
{
	check_length(1, "nullp", args);
	Cell* op = args -> get_car();
	op = eval(op, env);
  return new BoolCell(op == nil);
}

Cell* eval_listp(Cell* args, Environment* env)
{
	check_length(1, "listp", args);
	Cell* op = args -> get_car();
	op = eval(op, env);
  return new BoolCell(is_list(op));
}

Cell* eval_define(Cell* args, Environment* env)
{
  if (args == nil) {
    throw(runtime_error("bad syntax in define, nothing to define"));
  }
  assert(args -> is_cons());
  if (args -> get_level() != 1) {
    throw(runtime_error("local define is not allowed"));
  }
	Cell* key = args -> get_car();
	if (key == nil) {
		throw(runtime_error("bad syntax in define, key is nil"));
		return nil;
	}

  string var_name;
  Cell* val;
  if (key -> is_symbol()) {
    check_length(2, "define", args);
		var_name = key -> get_symbol();
		val = eval(args->get_cdr()->get_car(), env);
	} else if (key -> is_cons()) {
    if (key -> get_car() == nil || !(key -> get_car() -> is_symbol())) {
      throw(runtime_error("bad syntax in define, invalid procedure name"));
    }
    var_name = key -> get_car() -> get_symbol();
    Cell* lambda_cell = new ConsCell(key->get_cdr(), args->get_cdr(), 2);
    set_list_level(lambda_cell, 2);
    val = eval_lambda(lambda_cell, env);
  } else {
		throw(runtime_error("bad syntax in define"));
	}

  if (val != nil && val -> is_procedure()) {
    val -> set_name(var_name);
  }
  pair<string, Cell*> element(var_name, val);
  if (env -> empty() || (*env).front().find(var_name) != (*env).front().end()) {
    env -> push_front(SymbolTable());
  }
  (*env).front().insert(element);

	return nil;
}

Cell* retrieve_symbol(Cell* c, const Environment* env)
{
	assert(c != nil && c -> is_symbol());
	string s = c -> get_symbol();
	Environment::const_iterator env_it;
	for(env_it = env->begin(); env_it != env->end(); ++env_it) {
		SymbolTable::const_iterator symtab_it = env_it -> find(s);
		if (symtab_it != env_it -> end()) {
			return symtab_it -> second;
		}
	}
	if (is_builtin_func(s)) {
		return c;
	}
	throw(runtime_error("undefined symbol: " + s));
}

Cell* eval_lessthan(Cell* args, Environment* env)
{
	int num_of_args = list_length(args);
	if (num_of_args < 2) {
		throw(runtime_error("< needs at leasts two inputs"));
		return nil;
	}

	double prev_val = -numeric_limits<double>::max();
	while(args != nil) {
		Cell* op = eval(args -> get_car(), env);
		if (op == nil) {
			throw(runtime_error("Bad syntax in <"));
			return nil;
		}
		double curt_val;
		if (op -> is_int()) {
			curt_val = op -> get_int();
		}	else {
			curt_val = op -> get_double();
		}
		if (prev_val >= curt_val) {
			return new BoolCell(false);
		}
		prev_val = curt_val;
		args = args -> get_cdr();
	}
	return new BoolCell(true);
}

Cell* eval_not(Cell* args, Environment* env)
{
	check_length(1, "not", args);
	Cell* op = eval(args->get_car(), env);
  if (op != nil && op -> is_bool() && op -> get_bool() == false) {
    return new BoolCell(true);
  } else {
    return new BoolCell(false);
  }
}

Cell* eval_eval(Cell* args, Environment* env)
{
	check_length(1, "eval", args);
	Cell* op = eval(args->get_car(), env);
	return eval(op, env);
}

Cell* eval_print(Cell* args, Environment* env)
{
	check_length(1, "print", args);
	Cell* op = eval(args->get_car(), env);
	if(op == nil) {
		cout << "()" << endl;
	} else {
		op -> print();
	}
	return nil;
}

Cell* eval_lambda(Cell* const args, Environment* env)
{
	Cell* formals_it = args -> get_car();
	set<string> arg_names;
	while (formals_it != nil) {
		Cell* f = formals_it -> get_car();
		if (f == nil || !(f -> is_symbol())) {
			throw(runtime_error("lambda: formal arguments have to be identifier"));
		}
		string name = f -> get_symbol();
		pair<set<string>::iterator, bool> insert_ret = arg_names.insert(name);
		if (insert_ret.second == false) {
			throw(runtime_error("lambda: duplicate argument name in " + name));
		}
		formals_it = formals_it -> get_cdr();
	}

	Cell* body = args -> get_cdr();
	if (body == nil) {
		throw(runtime_error("no statement to build a procedure."));
	}
	// TODO: analyze body to identify free variables
	return new ProcedureCell(args->get_car(), body, env);
}

// void print_symbol_table(const SymbolTable& st)
// {
// 	for (SymbolTable::const_iterator it = st.begin(); it != st.end(); ++it) {
// 		cout << it -> first << ": ";
// 		if (it -> second == nil) {
// 			cout << "nil";
// 		} else {
// 			it -> second -> print();
// 		}
// 		cout << endl;
// 	}
// }
//
// void print_env(const Environment& env)
// {
// 	cout << "from up to down, printing environemnt: " << endl;
// 	for (Environment::const_iterator it = env.begin(); it != env.end(); ++it) {
// 		cout << "---layer start---" << endl;
// 		print_symbol_table(*it);
// 		cout << "---layer end---" << endl;
// 	}
// 	cout << "***********" << endl;
// }

Cell* eval_procedure(Cell* const proc, Cell* const args, Environment* env)
{
	assert(proc != nil);
	Cell* formals = proc -> get_formals();
	Cell* actuals = args;
	int len_formals = list_length(formals);
	int len_actuals = list_length(actuals);
	if (len_formals != len_actuals) {
		string err_msg = "procedure-call: wrong number of actual arguments\n\texpected:"
						+ to_string(len_formals) + "\n\tgiven:" + to_string(len_actuals);
		throw(runtime_error(err_msg));
	}
	// print_env(*env);

	SymbolTable local_map;
	while (formals != nil) {
		// don't need to check duplication, checked while constructing procedure
		Cell* evaled_actual = eval(actuals->get_car(), env);
		local_map.insert(pair<string, Cell*>(formals->get_car()->get_symbol(),
																				 evaled_actual));
		formals = formals -> get_cdr();
		actuals = actuals -> get_cdr();
	}
	// cout << "local_map: " << endl;
	// print_symbol_table(local_map);
	// cout << "-----" << endl;
	//
	string proc_name = proc -> get_name();
	if (proc_name != "" && local_map.find(proc_name) == local_map.end()) {
		local_map.insert(pair<string, Cell*>(proc_name, proc));
	}

	(proc->get_environment()).push_front(local_map);
	Cell* ret_val = nil;
	Cell* func_body = proc -> get_body();
	assert(func_body != nil);
	while (func_body != nil) {
		ret_val = eval(func_body->get_car(), &(proc->get_environment()));
		func_body = func_body -> get_cdr();
	}
	(proc->get_environment()).pop_front();
	return ret_val;
}

Cell* eval_apply(Cell* args, Environment* env)
{
	check_length(2, "apply", args);
	Cell* function = eval(args->get_car(), env);
	Cell* arg_list = eval(args->get_cdr()->get_car(), env);

	if(is_list(arg_list)) {
		return function -> apply(arg_list, env);
	} else {
		throw(runtime_error("bad syntax in apply"));
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


Cell* eval_subt(Cell* args, Environment* env)
{
  return eval_subt_divi<Subtraction>(args, env);
}

Cell* eval_divi(Cell* args, Environment* env)
{
  return eval_subt_divi<Division>(args, env);
}
