(define then-clause (cons (quote define) (cons (quote a) (cons 2 nil))))
(define else-clause (cons (quote define) (cons (quote a) (cons 3 nil))))
(define if-else-then
  (cons (quote if)
        (cons #t
              (cons then-clause
                    (cons else-clause nil)))))
(eval if-else-then)
;; Error: local define is not allowed

(if #f (define a 2) (define a 3))
;; Error: local define is not allowed

