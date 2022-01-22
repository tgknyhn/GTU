(setq VALUE "VALUE")
(setq IDENTIFIER "IDENTIFIER")
(setq COMMENT "COMMENT")
(setq word-list '())
(setq token-list '())
(setq all-chars  '())

;Keywords
(defvar KW_AND 		'("and")	)
(defvar KW_OR 		'("or")		)
(defvar KW_NOT 		'("not")	)
(defvar KW_EQUAL 	'("equal")	)
(defvar KW_LESS 	'("less")	)
(defvar KW_NIL 		'("nil")	)
(defvar KW_LIST 	'("list")	)
(defvar KW_APPEND 	'("append")	)
(defvar KW_CONCAT 	'("concat")	)
(defvar KW_SET 		'("set")	)
(defvar KW_DEFFUN 	'("deffun")	)
(defvar KW_FOR 		'("for")	)
(defvar KW_IF 		'("if")		)
(defvar KW_EXIT 	'("exit")	)
(defvar KW_LOAD 	'("load")	)
(defvar KW_DISP 	'("disp")	)
(defvar KW_TRUE 	'("true")	)
(defvar KW_FALSE 	'("false")	)


;; Main function (runs until user force quit)
(defun gppinterpreter ()
    (loop 
        (terpri)
        (princ "Enter: ")
		(setq all-chars (coerce (read-line) 'list))
		(handle-tokens all-chars)
		(print-reverse-list token-list)
		(setf word-list '())
		(setf token-list '())
		(setf all-list '())
    )
)

;; Prints given list in reverse order
(defun print-reverse-list (lst)
	(cond
		((null lst) '())
		(t
			(print-reverse-list (cdr lst))
			(write-line (car lst))
		)
	)
)

;; Puts every token to their type (keywords, operators, literals, identifiers)
(defun handle-tokens (all-chars)
	(loop for index in all-chars
		when T
			do (progn
				(if 
				(control-operator index) 
				() 
				(if (and (not (char= #\newline index)) (not (char= #\Tab index)) (not (char= #\Space index))) 	;
					(setq word-list (append word-list (list index)))
					(if (and (not (char= (first (last word-list)) #\Space)) (not (equal nil word-list)))
						(progn 
							(setq word-list (if(eq nil word-list) () (coerce word-list 'string)))
							(cond
								((numberp (read-from-string (coerce word-list 'string))) (setq token-list (push VALUE token-list))) 
								((control-keyword (list word-list)) () )
								((setq token-list (push IDENTIFIER token-list)))
							)
							(setf word-list '())
						)
					)
				)
				)
			)
	)
)

;; Checks given argument and pushs the keyword into token-list if it is equal to any g++ keyword
(defun control-keyword (keyword)
	(cond 
		( (equalp KW_AND 	keyword) 	(setq token-list (push "KW_AND" token-list))	)
		( (equalp KW_OR 	keyword) 	(setq token-list (push "KW_OR" token-list))		)
		( (equalp KW_NOT 	keyword)	(setq token-list (push "KW_NOT" token-list))	)
		( (equalp KW_EQUAL 	keyword) 	(setq token-list (push "KW_EQUAL" token-list))	)
		( (equalp KW_LESS 	keyword) 	(setq token-list (push "KW_LESS" token-list))	)
		( (equalp KW_NIL 	keyword) 	(setq token-list (push "KW_NIL" token-list))	)
		( (equalp KW_LIST 	keyword) 	(setq token-list (push "KW_LIST" token-list))	)
		( (equalp KW_APPEND keyword) 	(setq token-list (push "KW_APPEND" token-list))	)
		( (equalp KW_CONCAT keyword) 	(setq token-list (push "KW_CONCAT" token-list))	)
		( (equalp KW_SET 	keyword) 	(setq token-list (push "KW_SET" token-list))	)
		( (equalp KW_DEFFUN keyword) 	(setq token-list (push "KW_DEFFUN" token-list))	)
		( (equalp KW_FOR 	keyword) 	(setq token-list (push "KW_FOR" token-list))	)
		( (equalp KW_IF 	keyword) 	(setq token-list (push "KW_IF" token-list))		)
		( (equalp KW_EXIT 	keyword) 	(setq token-list (push "KW_EXIT" token-list))	)
		( (equalp KW_LOAD 	keyword) 	(setq token-list (push "KW_LOAD" token-list)) 	)
		( (equalp KW_DISP 	keyword) 	(setq token-list (push "KW_DISP" token-list))	)
		( (equalp KW_TRUE 	keyword) 	(setq token-list (push "KW_TRUE" token-list))	)
		( (equalp KW_FALSE 	keyword) 	(setq token-list (push "KW_FALSE" token-list)) 	)
	)	
)

;; Checks given argument and pushs the operator into token-list if it is equal to any g++ operator
(defun control-operator (op)
	(cond 
		( (string-equal #\+ op) (setq token-list (push "OP_PLUS" token-list))	)
		( (string-equal #\- op)	(setq token-list (push "OP_MINUS" token-list))	)
		( (string-equal #\/ op) (setq token-list (push "OP_DIV" token-list))	)
		( (string-equal #\* op) (setq token-list (push "OP_MULT" token-list))	)
		( (string-equal #\( op)	(setq token-list (push "OP_OP" token-list))		)
		( (string-equal #\) op) (setq token-list (push "OP_CP" token-list))		)
		( (string-equal #\‘ op) (setq token-list (push "OP_OC" token-list))		)
		( (string-equal #\’ op) (setq token-list (push "OP_CC" token-list))		)
		( (string-equal #\, op) (setq token-list (push "OP_COMMA" token-list))	)
	)
)

(gppinterpreter)
