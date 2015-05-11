(define > (lambda (x y) (< y x)))
(define >= (lambda (x y) (not (< x y))))
(define <= (lambda (x y) (not (< y x))))
(define = (lambda (x y) (if (< x y) 0 (not (< y x)))))
(define abs (lambda (x) (if (< x 0) (- 0 x) x)))
(define factorial (lambda (x) (if (< x 2) 1 (* x (factorial (- x 1))))))
(define square (lambda (x) (print (* x x))))
(define for-each
	(lambda (f list)
		(if (nullp list)
			(quote ())
			((lambda ()
				(f (car list))
				(for-each f (cdr list)))))))

(define list-tail
	(lambda (list k)
		(if k
			(list-tail (cdr list) (- k 1))
			list)))

(define list-ref
	(lambda (list k)
		(car (list-tail list k))))

(define list-append
	(lambda (l1 l2)
		(if (nullp l1)
			l2
			(cons (car l1) (list-append (cdr l1) l2)))))

(define element-append
	(lambda (l e)
		(list-append l (cons e ()))))

(define reverse
	(lambda (list)
		(if (nullp list)
			(quote ())
			(element-append (reverse (cdr list)) (car list)))))

(define ph
	(lambda (proc l l1 l2)
		(if (nullp l)
			(cons l1 (cons l2 ()))
			(if (proc (car l))
				(ph proc (cdr l) (element-append l1 (car l)) l2)
				(ph proc (cdr l) l1 (element-append l2 (car l)))))))

(define list-partition
	(lambda (proc list)
		(ph proc list (quote ()) (quote ()))))

(define list-sort
	(lambda (proc list)
		(if (nullp list)
			(quote ())
			((lambda ()
				(define pivot (list-ref list 0))
				(define ll (list-tail list 1))
				(define proc_1 (lambda (x) (proc x pivot)))
				(define lp (list-partition proc_1 ll))
				(list-append
					(element-append (list-sort proc (car lp)) pivot)
					(list-sort proc (car (cdr lp)))))))))

(define >5 (lambda (x) (> x 5)))

(define equal?
	(lambda (obj1 obj2)
		(if (listp obj1)
			(if (listp obj2)
				(if (nullp obj1)
					(if (nullp obj2)
						1
						0)
					(if (nullp obj2)
						0
						(if (equal? (car obj1) (car obj2))
							(equal? (cdr obj1) (cdr obj2))
							0)))
				0)
			(if (listp obj2)
				0
				(= obj1 obj2)))))

(define q (quote ((a 1) (b 2) (c 3))))

(define assoc
	(lambda (obj alist)
		(if (nullp alist)
			0
			(if (equal? obj (car (car alist)))
				(car alist)
				(assoc obj (cdr alist))))))
