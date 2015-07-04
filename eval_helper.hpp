#include "eval.hpp"

Cell* eval_ceiling(Cell* args, Environment* env);

Cell* eval_floor(Cell* args, Environment* env);

Cell* eval_if(Cell* args, Environment* env);

Cell* eval_quote(Cell* args);

Cell* eval_cons(Cell* args, Environment* env);

Cell* eval_car(Cell* args, Environment* env);

Cell* eval_cdr(Cell* args, Environment* env);

Cell* eval_nullp(Cell* args, Environment* env);

Cell* eval_listp(Cell* args, Environment* env);

Cell* eval_define(Cell* args, Environment* env);

Cell* retrieve_symbol(Cell* c, const Environment* env, bool* is_builtin=0);

Cell* eval_lessthan(Cell* args, Environment* env);

Cell* eval_not(Cell* args, Environment* env);

Cell* eval_eval(Cell* args, Environment* env);

Cell* eval_print(Cell* args, Environment* env);

Cell* eval_lambda(Cell* const args, Environment* env, std::string proc_name=ANONYMOUS_FUNC);

Cell* eval_procedure(Cell* const proc, Cell* const args, Environment* env);

Cell* eval_apply(Cell* args, Environment* env);

Cell* eval_plus(Cell* args, Environment* env);

Cell* eval_mult(Cell* args, Environment* env);

Cell* eval_subt(Cell* args, Environment* env);

Cell* eval_divi(Cell* args, Environment* env);

Cell* eval_plus(Cell* args, Environment* env);

Cell* eval_mult(Cell* args, Environment* env);

Cell* eval_subt(Cell* args, Environment* env);

Cell* eval_divi(Cell* args, Environment* env);

Cell* eval_let(Cell* args, Environment* env);
