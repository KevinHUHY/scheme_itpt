(define a 2.1)
(define b #f)
(floor (if (not b) a 3.2))
(ceil (if a a 3))

                                        ; (car (quote ())
                                        ; error in car
(define c (quote ()))
(nullp b)
(eval (quote (cons a 4)))
(define f (lambda (x) (+ x 1)))
(apply f (quote (2)))
; 3
(define cc (lambda (()) (+ 2 3)))
; bad syntax error should appear
(define g (lambda (x x) (+ x 1))
; error: lambda: duplicated argument name in x
((quote ()) 2)
                                        ; shouldn't be seg fault
(car (quote ()))
;(cdr (cons 2 (cons 3 ())))
;(define l (cons 2 (cons 3 (cons 4 ()))))
