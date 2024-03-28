(define p1 (list 1 2 3))
(define (len L)
	(if (zero? L)
			0
			(+ 1 (len (cdr L)))))
(len p1)
