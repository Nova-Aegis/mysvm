(define (num->int n)
  ((n (lambda (s) (+ s 1))) 0))
(define (evaluation f x n)
  (if (zero? n)
      x
      (evaluation f (f x) (- n 1))))

(define (num n)
  (lambda (f) (lambda (x) (evaluation f x n))))

(num->int (num 10))

(define succ
  (lambda (n)
    (lambda (f) (lambda (x)
                  ((n f) (f x))))))
(num->int (succ (num 5)))

(define plus
  (lambda (m n)
    (lambda (f)
      (lambda (x)
        ((m f) ((n f) x))))))
(num->int (plus (num 2) (num 3)))

(define mult
  (lambda (m n)
    (lambda (f)
      (lambda (x)
        ((m (n f)) x)))))
(num->int (mult (num 2) (num 6)))

(define pow
  (lambda (m n)
    (lambda (f)
      (lambda (x)
        (((n m) f) x)))))
(num->int (pow (num 2) (num 6)))

(define pred
  (lambda (n)
    (lambda (f)
      (lambda (x)
        (((n
           (lambda (g) (lambda (h) (h (g f)))))
          (lambda (u) x))
         (lambda (u) u))))))
(num->int (pred (num 5)))

(define sub
  (lambda (m n)
    (lambda (f)
      (lambda (x)
        ((((n pred) m) f) x)))))
(num->int (sub (num 5) (num 1)))
