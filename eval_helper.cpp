#include "eval_helper.hpp"
#include "arith.hpp"
#include <cmath>
#include <limits>
#include <string>
#include <set>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

class LevelGuard {
public:
  LevelGuard();
  ~LevelGuard();
  int get_level() const;
private:
  static int level;
};

int LevelGuard::level = 0;

LevelGuard::LevelGuard()
{
  assert(level >= 0);
  ++level;
}

LevelGuard::~LevelGuard()
{
  --level;
  assert(level >= 0);
}

int LevelGuard::get_level() const
{
  return level;
}

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
			|| s == "print" 	|| s == "lambda" 	|| s == "apply"
    	|| s == "let";
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
	if (op == nil || !(op -> is_double())) {
		throw(runtime_error(f + ": improper input"));
	}

	if(f == "ceiling") {
		return new IntCell(ceil(op->get_double()));
	} else {
		return new IntCell(floor(op->get_double()));
	}
}

Cell* eval_ceiling(Cell* args, Environment* env)
{
  LevelGuard lg;
	return eval_ceiling_floor(args, "ceiling", env);
}

Cell* eval_floor(Cell* args, Environment* env)
{
  LevelGuard lg;
	return eval_ceiling_floor(args, "floor", env);
}

// if-then-else, length must be 3
Cell* eval_if(Cell* args, Environment* env)
{
  LevelGuard lg;
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

//cons constructs conscell, but never sustitute the symbol
//should return correctly on the following input
//(cons 1 (cons (cons 2 nil) nil))
Cell* eval_cons(Cell* args, Environment* env)
{
  LevelGuard lg;
	check_length(2, "cons", args);
	Cell* op1 = args -> get_car();
	Cell* op2 = args -> get_cdr() -> get_car();
  op1 = eval(op1, env);
  op2 = eval(op2, env);
  Cell* ret = new ConsCell(op1, op2);
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
  LevelGuard lg;
	return eval_car_cdr(args, "car", env);
}

Cell* eval_cdr(Cell* args, Environment* env)
{
  LevelGuard lg;
	return eval_car_cdr(args, "cdr", env);
}

Cell* eval_nullp(Cell* args, Environment* env)
{
  LevelGuard lg;
	check_length(1, "nullp", args);
	Cell* op = args -> get_car();
	op = eval(op, env);
  return new BoolCell(op == nil);
}

Cell* eval_listp(Cell* args, Environment* env)
{
  LevelGuard lg;
	check_length(1, "listp", args);
	Cell* op = args -> get_car();
	op = eval(op, env);
  return new BoolCell(is_list(op));
}

bool define_lambda(Cell* val, Environment* env)
{
  if (val != nil && val -> is_cons()) {
    Cell* temp = eval(val->get_car(), env);
    if (temp != nil && temp -> is_symbol() && temp -> get_symbol() == string("lambda")) {
      return true;
    }
  }
  return false;
}

Cell* eval_define(Cell* args, Environment* env)
{
  LevelGuard lg;
  if (lg.get_level() != 1) {
    throw(runtime_error("local define is not allowed"));
  }
  if (list_length(args) < 2) {
    throw(runtime_error("bad syntax in define, too few arguments"));
  }
	Cell* key = args -> get_car();
	if (key == nil) {
		throw(runtime_error("bad syntax in define, key is nil"));
	}

  string name;
  Cell* val = nil;

  if (key -> is_symbol()) {
    check_length(2, "define", args);
		name = key -> get_symbol();
    if (is_builtin_func(name)) {
      throw(runtime_error("redefinition of built-in function is not allowed"));
    }
    val = args -> get_cdr() -> get_car();
    if (define_lambda(val, env)) {
      val = eval_lambda(val->get_cdr(), env, name);
    } else {
      val = eval(val, env);
    }
	} else if (key -> is_cons()) {
    if (key -> get_car() == nil || !(key -> get_car() -> is_symbol())) {
      throw(runtime_error("bad syntax in define, invalid procedure name"));
    }
    name = key -> get_car() -> get_symbol();
    if (is_builtin_func(name)) {
      throw(runtime_error("redefinition of built-in function is not allowed"));
    }
    Cell* lambda_cell = new ConsCell(key->get_cdr(), args->get_cdr());
    val = eval_lambda(lambda_cell, env, name);
  } else {
		throw(runtime_error("bad syntax in define, key is not a symbol"));
	}

  assert(env->size() == 1); //there should be exactly one SymbolTable in global_env
  pair<string, Cell*> element(name, val);
  SymbolTable::const_iterator search_handle = (*env).front().find(name);
  if (search_handle != (*env).front().end()) {
    (*env).front().erase(search_handle);
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

Cell* string_lessthan(string prev, Cell* args, Environment* env)
{
  while (args != nil) {
    Cell* op = eval(args -> get_car(), env);
    if (op == nil || !(op -> is_symbol())) {
      throw(runtime_error("comparison between Symbol and other types"));
    }
    if (prev >= op -> get_symbol()) {
      return new BoolCell(false);
    }
    prev = op -> get_symbol();
    args = args -> get_cdr();
  }
  return new BoolCell(true);
}

Cell* eval_lessthan(Cell* args, Environment* env)
{
  LevelGuard lg;
	int num_of_args = list_length(args);
	if (num_of_args < 2) {
		throw(runtime_error("< needs at leasts two inputs"));
		return nil;
	}
  Cell* first = eval(args -> get_car(), env);
  if (first != nil && first -> is_symbol()) {
    return string_lessthan(first->get_symbol(), args->get_cdr(), env);
  }

	double prev_val = -numeric_limits<double>::max();
	while(args != nil) {
		Cell* op = eval(args -> get_car(), env);
		if (op == nil) {
			throw(runtime_error("Bad syntax in <"));
		}
		double curt_val;
		if (op -> is_int()) {
			curt_val = op -> get_int();
		}	else if (op -> is_double()) {
			curt_val = op -> get_double();
		} else {
      throw(runtime_error("value is not comparable"));
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
  LevelGuard lg;
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
  //LevelGuard lg;
	check_length(1, "eval", args);
	Cell* op = eval(args->get_car(), env);
	return eval(op, env);
}

Cell* eval_print(Cell* args, Environment* env)
{
  LevelGuard lg;
	check_length(1, "print", args);
	Cell* op = eval(args->get_car(), env);
	if(op == nil) {
		cout << "()" << endl;
	} else {
		op -> print();
    cout << endl;
	}
	return nil;
}

void get_free_vars(const set<string>& defined_vars, const Cell* body, set<string>* free_vars)
{
  assert(free_vars != 0);
  assert(body != nil && body -> is_cons());
  while (body != nil) {
    Cell* curt_cell = body -> get_car();
    if (curt_cell != nil && curt_cell -> is_cons()) {
      get_free_vars(defined_vars, curt_cell, free_vars);
    } else if (curt_cell != nil && curt_cell -> is_symbol()) {
      string symbol = curt_cell -> get_symbol();
      if (defined_vars.find(symbol) == defined_vars.end() && !is_builtin_func(symbol)) {
        free_vars -> insert(symbol);
      }
    } else {}
    body = body -> get_cdr();
  }
}

Environment build_closure(const set<string>& arg_names, const Cell* func_body, Environment* env)
{
  set<string> symbols;
  Environment ret;
  SymbolTable symtab;
  // get_symbols(func_body, &symbols);
  get_free_vars(arg_names, func_body, &symbols);
  for (set<string>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
    Cell* val = nil;
    try {
      val = retrieve_symbol(new SymbolCell(it->c_str()), env);
      symtab.insert(pair<string, Cell*>(*it, val));
    } catch (runtime_error& e) {}
  }
  ret.push_front(symtab);
  return ret;
}

Cell* eval_lambda(Cell* const args, Environment* env, string proc_name)
{
  LevelGuard lg;
  if (list_length(args) < 2) {
    throw(runtime_error("lambda: bad syntax"));
  }
	Cell* formals_it = args -> get_car();
	set<string> arg_names; //also contains the function's name (if has)
  if (proc_name != ANONYMOUS_FUNC) {
    arg_names.insert(proc_name);
  }
	while (formals_it != nil) {
		Cell* f = formals_it -> get_car();
		if (f == nil || !(f -> is_symbol())) {
			throw(runtime_error("lambda: formal arguments have to be identifier"));
		}
		string name = f -> get_symbol();
		pair<set<string>::iterator, bool> insert_ret = arg_names.insert(name);
		if (insert_ret.second == false) {
			throw(runtime_error("lambda: duplicate argument name: " + name));
		}
		formals_it = formals_it -> get_cdr();
	}

	Cell* func_body = args -> get_cdr();
  assert(func_body != nil);

	// TODO: analyze func_body to identify free variables
  Environment closure_env = build_closure(arg_names, func_body, env);
	return new ProcedureCell(args->get_car(), func_body, proc_name, &closure_env);
}

void print_symbol_table(const SymbolTable& st)
{
	for (SymbolTable::const_iterator it = st.begin(); it != st.end(); ++it) {
		cout << it -> first << ": ";
		if (it -> second == nil) {
			cout << "nil";
		} else {
			it -> second -> print();
		}
		cout << endl;
	}
}

void print_env(const Environment& env)
{
  cout << "***********" << endl;
	for (Environment::const_iterator it = env.begin(); it != env.end(); ++it) {
		cout << "---layer start---" << endl;
		print_symbol_table(*it);
		cout << "---layer end---" << endl;
	}
	cout << "***********" << endl;
}

Cell* eval_procedure(Cell* const proc, Cell* const args, Environment* env)
{
  LevelGuard lg;
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
  // cout << "print evaluation environmet" << endl;
	// print_env(*env);
  // cout << "print closure" << endl;
  // print_env(proc->get_environment());

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

	string proc_name = proc -> get_name();
  // proc_name is guranteed to be different in eval_define
  assert(local_map.find(proc_name) == local_map.end());
	if (proc_name != ANONYMOUS_FUNC) {
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
  LevelGuard lg;
	check_length(2, "apply", args);
	Cell* function = eval(args->get_car(), env);
	Cell* arg_list = args->get_cdr()->get_car();

	if(is_list(arg_list)) {
		return function -> apply(arg_list, env);
	} else {
		throw(runtime_error("bad syntax in apply"));
	}
}

Cell* eval_plus(Cell* args, Environment* env)
{
  LevelGuard lg;
	return eval_plus_mult<Add>(args, env);
}

Cell* eval_mult(Cell* args, Environment* env)
{
  LevelGuard lg;
	return eval_plus_mult<Multiply>(args, env);
}

Cell* eval_subt(Cell* args, Environment* env)
{
  LevelGuard lg;
  return eval_subt_divi<Subtraction>(args, env);
}

Cell* eval_divi(Cell* args, Environment* env)
{
  LevelGuard lg;
  return eval_subt_divi<Division>(args, env);
}

Cell* eval_let(Cell* args, Environment* env)
{
  LevelGuard lg;
  if (list_length(args) < 2) {
    throw(runtime_error("let: bad syntax"));
  }
  env -> push_front(SymbolTable());
  Cell* id_vals = args -> get_car();
  Cell* body = args -> get_cdr();
  while (id_vals != nil) {
    Cell* pair = id_vals -> get_car();
    if (list_length(pair) != 2) {
      env -> pop_front();
      throw(runtime_error("let expression is ill-formed"));
    }
    Cell* id = pair -> get_car();
    if (!(id -> is_symbol())) {
      env -> pop_front();
      throw(runtime_error("let: id must be symbol"));
    }
    string key = id -> get_symbol();
    Cell* val = nil;
    try {
      val = pair -> get_cdr() -> get_car();
    } catch (runtime_error& e) {
      env -> pop_front();
      throw(e);
    }
    try {
      if (define_lambda(val, env)) {
        val = eval_lambda(val->get_cdr(), env, key);
      } else {
        val = eval(val, env);
      }
    } catch (runtime_error& e) {
      env -> pop_front();
      throw(e);
    }
    SymbolTable::iterator it = (*env).front().find(key);
    if (it != (*env).front().end()) {
      (*env).front().erase(it);
    }
    (*env).front().insert(std::pair<string, Cell*>(key, val));
    id_vals = id_vals -> get_cdr();
  }
  assert(body != nil);
  Cell* ret = nil;
  while (body != nil) {
    try {
      ret = eval(body -> get_car(), env);
    } catch (runtime_error& e) {
      env -> pop_front();
      throw(e);
    }
    body = body -> get_cdr();
  }
  env -> pop_front();
  return ret;
}
