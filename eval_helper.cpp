#include "eval_helper.hpp"
#include <cmath>
#include <limits>

using namespace std;

int list_length(Cell* c)
{
	int n = 0;
	while(c != nil) {
		c = c -> get_cdr();
		n++;
	}
	return n;
}

void check_length(int needed, string op, Cell* c)
{
	int n = list_length(c);
	if(n != needed) {
		stringstream ss;
		ss << "Wrong number of inputs for " << op << ". "
			 << needed << " is needed, " << n << " is provided.";
		throw(runtime_error(ss.str()));
	}
}

Cell* eval_and_substitute(Cell* c)
{
	if(listp(c)){
		c = eval(c);
	}
	if(c != nil && c -> is_symbol()) {
		string s = c -> get_symbol();
		if(s != "ceiling" && s != "floor" && s != "if" && s != "quote"
				&& s != "cons" && s != "car" && s != "cdr" && s != "car"
				&& s != "nullp" && s != "listp" && s != "+" && s != "-" && s != "*" && s != "/"
				&& s != "define" && s != "<" && s != "not" && s != "eval"
				&& s != "print" && s != "lambda" && s != "apply") {
			c = retrive_symbol(c -> get_symbol());
		}
	}
	return c;
}

//used by numerical function, < + - * /
Cell* get_operand(Cell* operand) throw(runtime_error)
{
	if(nullp(operand)) {
		string err_msg = "Non-numerical input";
		throw(runtime_error(err_msg));
	}
	operand = eval_and_substitute(operand);
	if(nullp(operand) || !operand -> is_number()) {
		string err_msg = "Non-numerical input";
		throw(runtime_error(err_msg));
	}
	return operand;
}

//ceiling and floor return numerical cell, no symbol!
Cell* deal_ceiling_floor(Cell* args, string op)
{
	//ceiling only accept one input.
	check_length(1, op, args);

	Cell* operand = args -> get_car();
	operand = eval_and_substitute(operand);
	double d = 0.0;
	//ceiling only accept double as input
	try{
		if(operand == nil) {
			throw(runtime_error("No input."));
		}
		d = get_double(operand);
	} catch(runtime_error& e) {
		string err_msg = op + " only accept double as operand.";
		throw(runtime_error(err_msg));
	}

	if(op == "ceiling") {
		return new IntCell(ceil(d));
	} else {
		return new IntCell(floor(d));
	}
}

//if returns a single cell, with symbol or number!
Cell* deal_if(Cell* args)
{
	int n = list_length(args);
	if(n != 2 && n != 3) {
		stringstream ss;
		ss << "Wrong number of inputs for if. 2 or 3 is needed, "
			 << n << " is provided.";
		throw(runtime_error(ss.str()));
	}

	Cell* predictor = args -> get_car();
	predictor = eval_and_substitute(predictor);

	Cell* ret = nil;
	if(predictor != nil && predictor -> is_number()
			&& predictor -> get_number() == 0) {
		//predictor is false, return the second cell
		if(n == 2) {
			return new IntCell(0);
		} else {
			ret = args -> get_cdr() -> get_cdr() -> get_car();
		}
	}	else {
		ret = args -> get_cdr() -> get_car();
	}
	// if(listp(ret)) {
	// 	ret = eval(ret);
	// }
	ret = eval_and_substitute(ret);
	return ret;
}

//quote never evals anything
Cell* deal_quote(Cell* args)
{
	check_length(1, "quote", args);
	return args -> get_car();
}

//cons constructs conscell, but never sustitute the symbol
Cell* deal_cons(Cell* args)
{
	check_length(2, "cons", args);
	Cell* operand1 = args -> get_car();
	Cell* operand2 = args -> get_cdr() -> get_car();
	if(!nullp(operand1)) {
		operand1 = eval(operand1);
	}
	if(!nullp(operand2)) {
		operand2 = eval(operand2);
	}
	return cons(operand1, operand2);
}

//do substitution?
Cell* deal_car(Cell* args)
{
	check_length(1, "car", args);
	Cell* operand = args -> get_car();
	operand = eval(operand);
	if(operand == nil) {
		throw(runtime_error("error in car."));
	}
	return operand -> get_car();
}

//do substitution
Cell* deal_cdr(Cell* args)
{
	check_length(1, "cdr", args);
	Cell* operand = args -> get_car();
	operand = eval(operand);
	if(operand == nil) {
		throw(runtime_error("error in cdr."));
	}
	return operand -> get_cdr();
}

//do substitution
//i.e (define b (quote ())); (nullp b) = 1
Cell* deal_nullp(Cell* args)
{
	check_length(1, "nullp", args);
	Cell* operand = args -> get_car();
	if(!nullp(operand)) {
		operand = eval(operand);
	}
	return new IntCell((int)nullp(operand));
}

Cell* deal_listp(Cell* args)
{
	check_length(1, "listp", args);
	Cell* operand = args -> get_car();
	if(!nullp(operand)) {
		operand = eval(operand);
	}
	return new IntCell((int)listp(operand));
}

//do evaluation before store the value
Cell* deal_define(Cell* args) throw(runtime_error)
{
	check_length(2, "define", args);
	string symbol;
	try {
		symbol = args -> get_car() -> get_symbol();
	} catch(runtime_error& e) {
		string err_msg = "In define. ";
		err_msg += e.what();
		throw(runtime_error(err_msg));
	}
	Cell* value = args -> get_cdr() -> get_car();
	// if do substitution
	// if(value -> is_symbol()) {
	// 	list< bstmap< string, Cell* > >::iterator it;
	// 	for(it = sym_table_list.begin(); it != sym_table_list.end(); ++it) {
	// 		ret = it -> find(s);
	// 		if(ret != it -> end()) {
	// 			break;
	// 		}
	// 	}
	// 	if(it != sym_table_list.end()) {
			// value = eval_and_substitute(value);
	// 	}
	// }
	// if not
	//if(listp(value)) {
		value = eval(value);
	//}
	if(sym_table_list.empty()) {
		//map< string, Cell* > cur_table; // anyway to simplify?
		sym_table_list.push_front(bstmap< string, Cell* > ());
		// sym_table_list.emplace_front();
	}
	pair< bstmap<string, Cell*>::iterator, bool > ret;
	ret = sym_table_list.front().insert(pair<string, Cell*>(symbol, value));
	if(ret.second == false) {
		string err_msg = "Try to redefine ";
		err_msg += symbol;
		throw(runtime_error(err_msg));
	}
	return nil;
}

// user should check whether the symbol can be retrieved
Cell* retrieve_symbol(string s, const Environment* env)
{
	SymbolTable::iterator symtab_it;
	Environment::iterator env_it;
	for(env_it = env -> begin(); env_it != env -> end(); ++env_it) {
		symtab_it = env_it -> find(s);
		if (symtab_it != it -> end()) {
			return symtab_it -> second;
		}
	}
	return nil;
}

Cell* deal_lessthan(Cell* args) throw(runtime_error)
{
	bool ans = true;
	Cell* operand = nil;
	double old_value = -numeric_limits<double>::max();
	double new_value = 0;
	string old_symbol = "_INIT_";
	while(args != nil) {
		operand = eval(args -> get_car());
		if(operand -> is_number()) {
			new_value = operand -> get_number();
			ans = ans && (old_value < new_value);
			old_value = new_value;
		} else if(operand -> is_symbol()) {
			if(old_symbol != "_INIT_") {
				ans = ans && (old_symbol < operand -> get_symbol());
			}
			old_symbol = operand -> get_symbol();
		} else {
			throw(runtime_error("something bad happens in <"));
		}
		if(ans == false) {
			break;
		}
		args = args -> get_cdr();
	}
	return new IntCell(ans);
}

//do substitution, otherwise symbol is a bug
//but then (not +) is a bug
Cell* deal_not(Cell* args) throw(runtime_error)
{
	check_length(1, "not", args);
	Cell* operand = eval_and_substitute(args -> get_car());
	if(operand != nil && operand -> is_number() && operand -> get_number() == 0){
		return new IntCell(1);
	} else {
		return new IntCell(0);
	}
}

Cell* deal_eval(Cell* args) throw(runtime_error)
{
	check_length(1, "eval", args);
	Cell* operand = eval_and_substitute(args -> get_car());
	if(operand != nil && operand -> is_cons()) {
		operand = eval_and_substitute(operand);
	}
	return operand;
}

//in the sample program, no substitution is done.
Cell* deal_print(Cell* args) throw(runtime_error)
{
	check_length(1, "print", args);
	Cell* operand = eval(args -> get_car());
	if(operand == nil) {
		cout << "()" << endl;
	}
	else {
		cout << *operand << endl;
	}
	return nil;
}

Cell* deal_lambda(Cell* args) throw(runtime_error)
{
	Cell* formal = args -> get_car();
	Cell* body = args -> get_cdr();
	if(body == nil) {
		throw(runtime_error("No statement to build a procedure."));
	}
	return new ProcedureCell(formal, body);
}

Cell* deal_procedure(Cell* const proc, Cell* const args) throw(runtime_error)
{
	bstmap< string, Cell* > local_map;
	Cell* formal_arg = proc -> get_formals();
	Cell* actual_arg = args;
	pair< bstmap<string, Cell*>::iterator, bool > ret;
	if(list_length(formal_arg) != list_length(actual_arg)) {
		cout << "# formal_arg: " << list_length(formal_arg) << endl;
		cout << "# actual_arg: " << list_length(actual_arg) << endl;
		throw(runtime_error("Wrong number of actual arguments."));
	}
	while(formal_arg != nil && actual_arg != nil) {
		ret = local_map.insert(	pair< string, Cell* >
														(formal_arg -> get_car() -> get_symbol(),
														eval(actual_arg -> get_car()))
													);	//!!! why eval instead of e_and_s???
		if(ret.second == false) {
			string err_msg = "Try to redefine ";
			err_msg += formal_arg -> get_car() -> get_symbol();
			throw(runtime_error(err_msg));
		}
		formal_arg = formal_arg -> get_cdr();
		actual_arg = actual_arg -> get_cdr();
	}
	sym_table_list.push_front(local_map);
	Cell* ret_val = nil;
	Cell* func_body = proc -> get_body();
	while(func_body != nil) {
		ret_val = eval(func_body -> get_car());
		// ret_val = eval_and_substitute(func_body -> get_car());
		func_body = func_body -> get_cdr();
	}
	sym_table_list.pop_front();
	return ret_val;
}

Cell* deal_apply(Cell* args) throw(runtime_error)
{
	check_length(2, "apply", args);
	Cell* proc_cell = args -> get_car();
	proc_cell = eval(proc_cell);
	Cell* args_for_proc = args -> get_cdr() -> get_car();
	if(listp(args_for_proc)) {
		args_for_proc = eval(args_for_proc);
	}
	return proc_cell -> apply(args_for_proc);
}

Cell* deal_plus(Cell* args)
{
	double result = 0;
	bool is_double = false;
	Cell* operand = nil;
	while(args != nil) {
		try {
			operand = get_operand(args -> get_car());
		} catch(runtime_error& e) {
			string err_msg(e.what());
			err_msg += " for +";
			throw(runtime_error(err_msg));
		}
		result += operand -> get_number();
		is_double += operand -> is_double();
		args = args -> get_cdr();
	}
	if(is_double) {
		return new DoubleCell(result);
	}
	else {
		return new IntCell(result);
	}
}

Cell* deal_mult(Cell* args)
{
	double result = 1;
	bool is_double = false;
	Cell* operand = nil;
	while(args != nil) {
		try {
			operand = get_operand(args -> get_car());
		} catch (runtime_error& e) {
			string err_msg(e.what());
			err_msg += " for *";
			throw(runtime_error(err_msg));
		}
		result *= operand -> get_number();
		is_double += operand -> is_double();
		args = args -> get_cdr();
	}
	if(is_double) {
		return new DoubleCell(result);
	}
	else {
		return new IntCell(result);
	}
}

Cell* deal_subt(Cell* args)
{
	if(list_length(args) == 0) {
		string err_msg = "Empty input for -";
		throw(runtime_error(err_msg));
	}
	//get the first operand
	Cell* operand = nil;
	try {
		operand = get_operand(args -> get_car());
	} catch (runtime_error& e) {
		string err_msg(e.what());
		err_msg += "for -";
		throw(runtime_error(err_msg));
	}
	//if there is only one operand, return the inverse
	if(list_length(args) == 1) {
		double number = 0 - operand -> get_number();
		if(operand -> is_double()) {
			return new DoubleCell(number);
		} else {
			return new IntCell(number);
		}
	}
	//else, the first the operand is the minuend
	double result = operand -> get_number();
	bool is_double = operand -> is_double();
	while((args = args -> get_cdr()) != nil) {
		try {
			operand = get_operand(args -> get_car());
		} catch(runtime_error& e) {
			string err_msg(e.what());
			err_msg += " for -";
			throw(runtime_error(err_msg));
		}
		result -= operand -> get_number();
		is_double += operand -> is_double();
	}
	if(is_double) {
		return new DoubleCell(result);
	}
	else {
		return new IntCell(result);
	}
}

Cell* deal_divi(Cell* args)
{
	if(list_length(args) == 0) {
		string err_msg = "Empty input for /";
		throw(runtime_error(err_msg));
	}
	//get the first operand
	Cell* operand = nil;
	try {
		operand = get_operand(args -> get_car());
	} catch (runtime_error& e) {
		string err_msg(e.what());
		err_msg += "for *";
		throw(runtime_error(err_msg));
	}
	//if there is only one operand, return the inverse
	if(list_length(args) == 1) {
		if(operand -> get_number() == 0) {
			throw(runtime_error("Divisor cannot be 0"));
		}
		double number = 1.0 / operand -> get_number();
		return make_double(number);
	}
	//else, the first the operand is the minuend
	double result = operand -> get_number();
	bool is_double = operand -> is_double();
	while((args = args -> get_cdr()) != nil) {
		try {
			operand = get_operand(args -> get_car());
		} catch(runtime_error& e) {
			string err_msg(e.what());
			err_msg += " for /";
			throw(runtime_error(err_msg));
		}
		is_double += operand -> is_double();
		if(operand -> get_number() == 0) {
			throw(runtime_error("Divisor cannot be 0"));
		}
		if(is_double) {
			result = result / operand -> get_number();
		} else {
			result = ((int)result)/((int) operand -> get_number());
		}
	}
	if(is_double) {
		return new DoubleCell(result);
	}
	else {
		return new IntCell(result);
	}
}
