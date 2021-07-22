;
; Копирайт (С) 2019, ООО «Нанософт разработка». Все права защищены.
; 
; Данное программное обеспечение, все исключительные права на него, его
; документация и сопроводительные материалы принадлежат ООО «Нанософт разработка».
; Данное программное обеспечение может использоваться при разработке и входить
; в состав разработанных программных продуктов при соблюдении условий
; использования, оговоренных в «Лицензионном договоре присоединения
; на использование программы для ЭВМ «Платформа nanoCAD»».
; 
; Данное программное обеспечение защищено в соответствии с законодательством
; Российской Федерации об интеллектуальной собственности и международными
; правовыми актами.
; 
; Используя данное программное обеспечение,  его документацию и
; сопроводительные материалы вы соглашаетесь с условиями использования,
; указанными выше. 
;

 ; Parse string (divider div)

(defun parstr (str div / i c ll name)
  (while (wcmatch str (strcat "*" div "*"))
    (setq i 1
	  name ""
    )
    (while (/= (setq c (substr str i 1)) div)
      (setq i (1+ i))
      (setq name (strcat name c))
    )
    (setq ll (append ll (list (atoi name))))
    (setq str (substr str (1+ i)))
  )
  (append ll (list (atoi str)))
)

 ; Acquire information for covering wall your tiles 
(defun input (/ owall hwall wwall tstrdims tlistdims tspac)
 ;point can't be null
  (initget 17)
  (setq owall (getpoint "\nLower left corner of the wall :"))

  (if (null *hwall_default*)
    (setq *hwall_default* 2800)
  )
  (while (or (null hwall) (> hwall 2800))
    (initget 6) ;Height can't be 0 or neg
    (setq hwall	(getdist (strcat "\nHeight of the wall <"
				 (rtos *hwall_default* 2 0)
				 ">: "
			 )
			 owall
		)
    )
    (if	(null hwall)
      (setq hwall *hwall_default*)
      (if (> hwall 2800)
	(prompt "\nHeight must not be greater than 2800")

      )
    )
  )
  (setq *hwall_default* hwall)

  (if (null *wwall_default*)
    (setq *wwall_default* 4000)
  )
  (while (or (null wwall) (> wwall 4000))
    (initget 6)
    (setq wwall	(getdist (strcat "\nWidth of the wall <"
				 (rtos *wwall_default* 2 0)
				 ">: "
			 )
			 owall
		)
    )
    (if	(null wwall)
      (setq wwall *wwall_default*)
      (if (> wwall 4000)
	(prompt "\nWidth must not be greater than 4000")

      )
    )
  )
  (setq *wwall_default* wwall)

  (initget "100x100 200x300 300x600")
  (setq
    tstrdims
     (getkword
       "\nDimensions of ceramic tiles [100x100/200x300/300x600] <200x300>: "
     )
  )
  (if (null tstrdims)
    (setq tstrdims "200x300")
  )
  (setq tlistdims (parstr tstrdims "x"))

  (while (or (null tspac) (> tspac 6))
    (initget 4)
    (setq tspac (getreal "\nSpacing between ceramic tiles <2>: "))
    (if	(null tspac)
      (setq tspac 2)
      (if (> tspac 6)
	(prompt "\Spacing must not be greater than 6")

      )
    )
  )

  (list owall wwall hwall tlistdims tspac)
)


 ; Draw recntangle (wall, tile)
(defun drawrect
       (origin width hight / p1 p2 p3 p4 arraySpace sArray plRectan point_lst)
  (setq p1 (polar origin (/ pi 2) hight))
  (setq p2 (polar p1 0 width))
  (setq p3 (polar p2 (/ pi -2) hight))
  (setq p4 (polar p3 (- pi) width))

  (cond
    ((= type_constr "Command")
     (command "pline" p1 p2 p3 p4 "c")
    )
    ((= type_constr "Entmake")
     (entmake (list '(0 . "LWPOLYLINE")
		    '(100 . "AcDbEntity")
		    '(100 . "AcDbPolyline")
		    ;;constant width
		    (cons 43 0)
		    ;;in rectangle 4 points
		    (cons 90 4)
		    (cons 10 p1)
		    (cons 10 p2)
		    (cons 10 p3)
		    (cons 10 p4)
		    ;;closed pline
		    '(70 . 1)
	      )
     )
    )
    ((= type_constr "ActiveX")
     (setq point_lst (list (car p1)
			   (cadr p1)
			   (car p2)
			   (cadr p2)
			   (car p3)
			   (cadr p3)
			   (car p4)
			   (cadr p4)
		     )
     )
     (setq arraySpace
 ; allocate space for an array of 2d points stored as doubles
	    (vlax-make-safearray
	      vlax-vbdouble ; element type
	      (cons 0 (1- (length point_lst))) ; array dimension
	    )
     )
     (setq sArray (vlax-safearray-fill
		    arraySpace
		    (list (car p1)
			  (cadr p1)
			  (car p2)
			  (cadr p2)
			  (car p3)
			  (cadr p3)
			  (car p4)
			  (cadr p4)
		    )
		  )
     )

     (setq
       plRectan (vla-addLightweightPolyline
	       *ModelSpace*
	       (vlax-make-variant sArray)
	     )
     )
     (vla-put-closed plRectan T)
    )
  )
)


 ; Draw the rows of tiles 
(defun drawtiles (list_dt   /	      origin	wwidth	  wheight
		  width	    height     tspac	pheight	  pwidth
		  curorig   count
		 )
  (setq origin (car list_dt))
  (setq wwidth (cadr list_dt))
  (setq wheight (caddr list_dt))

  (setq width (car (nth 3 list_dt)))
  (setq height (cadr (nth 3 list_dt)))
  (setq tspac (nth 4 list_dt))

  (setq	pheight	0
	curorig	origin
	count	0
  )

  (while (< pheight wheight)
    (setq pwidth 0)
    (while (< pwidth wwidth)
      (drawrect curorig width height)
      (setq pwidth (+ pwidth width tspac)
	    count  (1+ count)
      )
      (setq curorig (polar curorig 0 (+ width tspac)))
    )
    (setq pheight (+ pheight height tspac))
    (setq curorig (polar origin (/ pi 2) pheight))
  )
  (list count (* count width height 1E-6))
)


 ; Main command
(defun C:TILES	(/	       list_data     old_blip old_osnap
		 old_cmde      type_constr   *ModelSpace*
		 old_ccol      cnt_&_sqr
		)
  (setq list_data (input))
  (setq old_blip (getvar "blipmode"))
  (setq old_osnap (getvar "osmode"))
  (setq old_cmde (getvar "cmdecho"))
  (setvar "blipmode" 0)
  (setvar "cmdecho" 0)
  (setvar "osmode" 0)

  (initget "Command Entmake ActiveX")
  (setq	type_constr
	 (getkword
	   "\nType of construction [Command/Entmake/ActiveX] <Entmake>: "
	 )
  )
  (if (null type_constr)
    (setq type_constr "Entmake")
  )

  (if (= type_constr "ActiveX")
    (progn
      (vl-load-com)
      (setq *ModelSpace*
	     (vla-get-ModelSpace
	       (vla-get-ActiveDocument (vlax-get-Acad-Object))
	     )
      )
    )
  )

  (setq old_ccol (getvar "cecolor"))
  (setvar "cecolor" "4")
  (drawrect (car list_data)
	    (cadr list_data)
	    (caddr list_data)
  )
  (setvar "cecolor" old_ccol)

  ;(setq sec0 (* (getvar "tdusrtimer") 86400))
  (setq cnt_&_sqr (drawtiles list_data))
  ;(print (- (* (getvar "tdusrtimer") 86400) sec0))

  (setvar "blipmode" old_blip)
  (setvar "cmdecho" old_cmde)
  (setvar "osmode" old_osnap)

  (princ (strcat "\nYou will need "
		 (rtos (car cnt_&_sqr) 2 0)
		 " ceramic tiles ("
		 (rtos (cadr cnt_&_sqr) 2 1)
		 " sq.m.)."
	 )
  )

  (princ)
)
