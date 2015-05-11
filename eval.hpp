#ifndef __EVAL__
#define __EVAL__
#include "Cell.hpp"

/**
 * \brief Evaluate the expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return a pointer to a dynamic Cell storing the result.
 */
Cell* eval(Cell* c, Environment* globe_env);

#endif /* defined(__EVAL__) */
