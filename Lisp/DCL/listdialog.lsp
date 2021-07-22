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

; dialog with simple logic - you can add items to listbox
(defun listdialog ()
  ; loading and creating dialog
  (setq dcl (load_dialog "listdialog.dcl"))
  (new_dialog "listdialog" dcl)
  ; let's set two items for list
  (start_list "list")
  (add_list "item 1")
  (add_list "item 2")
  (end_list)
  ; set actions
  ; when user clicks on "add" button, we must add value of
  ; "edit" tile to "list" tile
  ; when user clicks on "clear" button, we must clear contents of "edit" tile
  ; note that in that case we must use start_list with 2 as second argument,
  ; otherwise all contents will be erased
  (start_list "list" 2)
  (add_list (get_tile "edit"))
  (end_list)

  ; set actions for Add and Clear buttons
  (action_tile "add" (strcat
                      "(start_list \"list\" 2)"
                      "(add_list (get_tile \"edit\"))"
                      "(end_list)"))
  (action_tile "clear" (strcat
                        "(start_list \"list\")"
                        "(end_list)"))
  ; now start dialog
  (start_dialog)
  (unload_dialog dcl))

(defun c:listdialog ()
  (listdialog))
