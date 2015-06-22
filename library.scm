(define nil (quote ()))

(define > (lambda (x y) (< y x)))

(define >= (lambda (x y) (not (< x y))))

(define <= (lambda (x y) (not (< y x))))

(define = (lambda (x y) (if (< x y) #f (not (< y x)))))

(define abs (lambda (x) (if (< x 0) (- 0 x) x)))

(define factorial (lambda (x) (if (< x 2) 1 (* x (factorial (- x 1))))))

(define square (lambda (x) (* x x)))

(define for-each
	(lambda (f list)
		(if (nullp list)
        (quote ())
        ((lambda ()
           (f (car list))
           (for-each f (cdr list)))))))

(define (map f ls)
  (if (nullp ls)
      (quote ())
      (cons (f (car ls))
            (map f (cdr ls)))))

(define ls (quote (1 2 3 4 5)))

(define list-tail
	(lambda (list k)
    (if (< k 0)
        (print ("Error: k should >= 0"))
        (if (= k 0)
            list
            (list-tail (cdr list) (- k 1))))))

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
		(list-append l (cons e (quote ())))))

(define reverse
	(lambda (list)
		(if (nullp list)
        (quote ())
        (element-append (reverse (cdr list)) (car list)))))

(define (list-partition proc ls)
  (let ((ph (lambda (ls ls1 ls2)
             (if (nullp ls)
                 (cons ls1 (cons ls2 (quote ())))
                 (if (proc (car ls))
                     (ph (cdr ls) (cons (car ls) ls1) ls2)
                     (ph (cdr ls) ls1 (cons (car ls) ls2)))))))
    (ph ls (quote ()) (quote ()))))

(define (>5 x) (> x 5))

(define (hd ls)
  (list-ref ls 0))

(define (tl ls)
  (list-tail ls 1))

(define (list-sort proc ls)
  (if (nullp ls)
      (quote ())
      (let ((lp (list-partition (lambda (x) (proc x (hd ls))) (tl ls))))
        (list-append (element-append (list-sort proc (car lp)) (hd ls))
                     (list-sort proc (car (cdr lp)))))))

(define equal?
	(lambda (obj1 obj2)
		(if (listp obj1)
			(if (listp obj2)
          (if (nullp obj1)
              (if (nullp obj2)
                  #t
                  #f)
              (if (nullp obj2)
                  #f
                  (if (equal? (car obj1) (car obj2))
                      (equal? (cdr obj1) (cdr obj2))
                      #f)))
				#f)
			(if (listp obj2)
          #f
          (= obj1 obj2)))))

(define q (quote ((a 1) (b 2) (c 3))))

(define assoc
	(lambda (obj alist)
		(if (nullp alist)
        #t
        (if (equal? obj (car (car alist)))
            (car alist)
            (assoc obj (cdr alist))))))
