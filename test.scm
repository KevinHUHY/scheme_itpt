(define a 2.1)
(define b 0)
(floor (if (not b) a 3.2))
; 2
(car (quote ())
; error in car
(define b (quote ()))
;(nullp b) = 1
(eval (quote (cons a 4)))
; (2.1 .4)
(define f (lambda (x) (+ x 1)))
(apply f (quote (2)))
; 3
(define cc (lambda (()) (+ 2 3)))
(cc 2)
; shouldn't be seg fault
(define g (lambda (x x) (+ x 1))
; error: lambda: duplicated argument name in x
((quote ()) 2)
; shouldn't be seg fault
