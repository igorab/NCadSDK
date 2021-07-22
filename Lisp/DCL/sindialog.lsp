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

; note that this .lsp depends on messagebox.lsp
; so messagebox.lsp must be loaded before you load sindialog.lsp

; degree to radians
(defun deg2rad (deg)
  (/ deg (/ 180 pi)))

; check if string is number
(defun is-number (s)
  (numberp(read s)))

; show error message
; string is not a number
(defun not-number-error (s)
  (show-message-box "not a number" (strcat "Input string is not a number: " s)))

(defun calculate-sin ()
  (setq sin-arg (get_tile "sin_edit"))
  (if (is-number sin-arg)
    (progn
      (setq sin-res (rtos (sin (deg2rad (atof sin-arg)))))
      (set_tile "sin_result" sin-res)
      (start_list "history" 2)
      (add_list (strcat "sin(" sin-arg ") = " sin-res))
      (end_list))
    (not-number-error sin-arg)))

; dialog for calculating sinus
(defun sindialog ()
  (setq dcl (load_dialog "sindialog.dcl"))
  (new_dialog "sindialog" dcl)
  ; calculate sin on sin_btn click
  (action_tile "sin_btn" "(calculate-sin)")
  (start_dialog)
  (unload_dialog dcl))

(defun c:sindialog ()
  (sindialog))
