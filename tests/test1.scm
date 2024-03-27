(define (construct-list n)
	(if (zero? n)
			(list)
			(cons n (construct-list (- n 1)))))

(construct-list 3)

(define (triangle n)
	(if (zero? n)
			(list)
			(cons (construct-list n) (triangle (- n 1)))))

(triangle 4)
