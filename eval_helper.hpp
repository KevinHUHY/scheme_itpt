#include "eval.hpp"
using namespace std;

/**
  * \brief helper function
  *				dealing with a list starting from a symbol "ceiling" or "floor"
  * \param c pointer to a list
  * \return a newly created cell storing the result
  */
Cell* deal_ceiling_floor(Cell* c, string op);

/**
  * \brief helper function
  *				dealing with a list starting from a symbol "if"
  * \param c pointer to a list
  * \return a newly created cell storing the result
  */
Cell* deal_if(Cell* c);

/**
  * \brief helper function
  *				dealing with a list starting from a symbol "quote"
  * \param c pointer to a list
  * \return the operand as not evaluated s-expression
  */
Cell* deal_quote(Cell* c);

/**
  * \brief helper function
  *				dealing with a list starting from a symbol "cons"
  * \param c pointer to a list
  * \return constructed list.
  */
Cell* deal_cons(Cell* c);

/**
 * \brief helper function
 *				dealing with a list starting from a symbol "car"
 * \param c pointer to a list
 * \return the car of the following list.
 */
 Cell* deal_car(Cell* c);

 /**
 * \brief helper function
 *				dealing with a list starting from a symbol "cdr"
 * \param c pointer to a list
 * \return the cdr of the following list.
 */
Cell* deal_cdr(Cell* c);

/**
 * \brief helper function
 *				dealing with a list starting from a symbol "nullp"
 * \param c pointer to a list
 * \return 0 if following list is empty.
 */
Cell* deal_nullp(Cell* c);

/**
 * \brief helper function
 *				dealing with a list starting from a symbol "listp"
 * \param c pointer to a list
 * \return 1 if arg is a list.
 */
Cell* deal_listp(Cell* c);

/**
 * \brief helper function
 					to get and check operand for + - * /
 * \param c pointer to a car of a conspair
 * \return the real operand, either IntCell or DoubleCell
 					 if all check is passed
 */
Cell* get_operand(Cell* operand) throw(runtime_error);

/**
 * \brief helper function
 *				dealing with a list starting from a symbol +
 * \param c pointer to a list
 * \return a newly created cell storing the result
 */
Cell* deal_plus(Cell* c);

/**
 * \brief helper function
 *				dealing with a list starting from a symbol *
 * \param c pointer to a list
 * \return a newly created cell storing the result
 */
 Cell* deal_mult(Cell* c);

 /**
 * \brief helper function
 *				dealing with a list starting from a symbol -
 * \param c pointer to a list
 * \return a newly created cell storing the result
 */
Cell* deal_subt(Cell* c);

/**
 * \brief helper function
 *				dealing with a list starting from a symbol /
 * \param c pointer to a list
 * \return a newly created cell storing the result
 */
Cell* deal_divi(Cell* c);

/**
 * \brief helper function
 *				dealing with command define/
 * \param c pointer to a list
 * \return nil
 * @exception<runtime_error> runtime_error when redefinition happens
 */
Cell* deal_define(Cell* c) throw(runtime_error);

/**
 * \brief helper function
 *				dealing with a list starting from a symbol /
 * \param c pointer to a list
 * \return a newly created cell storing the result
  */
 Cell* retrieve_symbol(string s, const Environment* env);

/**
 * \brief helper function
 *				dealing with a list starting from < /
 * \param c pointer to a list
 * \return a newly created cell storing the result
 * @exception<runtime_error> runtime_error when input is not number
 */
Cell* deal_lessthan(Cell* c) throw(runtime_error);

/**
 * \brief helper function
 *				dealing with a list starting from not /
 * \param c pointer to a list
 * \return the negation of the operand
 * @exception<runtime_error> runtime_error when there are more than one operands
 */
Cell* deal_not(Cell* c) throw(runtime_error);

/**
 * \brief helper function      dealing with a list starting from eval
 * \param c pointer to a list
 * \return a cell store the result of evaluation
 * @exception<runtime_error> runtime_error when there is a undefined symbol
 */
Cell* deal_eval(Cell* c) throw(runtime_error);

/**
 * \brief helper function      dealing with a list starting from lambda
 * \param c pointer to a list
 * \return a cell store the result of evaluation
 * @exception<runtime_error> runtime_error when there is no procedure statement
 */
Cell* deal_lambda(Cell* c) throw(runtime_error);

Cell* deal_procedure(Cell* const proc, Cell* const c) throw(runtime_error);

Cell* deal_apply(Cell* c) throw(runtime_error);

/**
 * \brief helper function      dealing wiht a list starting from print
 * \param c pointer to a list
 * \return nil and print out the evaluated result
 * @exception<runtime_error> runtime_error when the list cannot be evaluated
 */
Cell* deal_print(Cell* c) throw(runtime_error);
