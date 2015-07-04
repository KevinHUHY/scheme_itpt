This project started from a course project for object-oriented program in HKUST,
where we developed a micro scheme interpreter under R5S5 standard on the journey
of learning C++ and oo design. The original design and implementation was more
defective since I was not familiar with Scheme at that time. The current
implementation was an overhaul of the old one after I learnt much more about
programming lanuage and compiler and interpreter design.
During the rewriten, I restructured the entire program, eliminated many pointer
flaws that can cause segmentation fault when the inputs are delibrately ill-
formed, leveraged template function and functor in C++ to simplify the evaluation
function of arithmatic operation, added support for lexical scope and closure and
later improved the efficiency of closure by identifying free variables.
My future plan would be adding hygienic macro and improve memory efficiency
