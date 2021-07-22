;
; �������� (�) 2019, ��� ��������� ����������. ��� ����� ��������.
; 
; ������ ����������� �����������, ��� �������������� ����� �� ����, ���
; ������������ � ���������������� ��������� ����������� ��� ��������� ����������.
; ������ ����������� ����������� ����� �������������� ��� ���������� � �������
; � ������ ������������� ����������� ��������� ��� ���������� �������
; �������������, ����������� � ������������� �������� �������������
; �� ������������� ��������� ��� ��� ���������� nanoCAD��.
; 
; ������ ����������� ����������� �������� � ������������ � �����������������
; ���������� ��������� �� ���������������� ������������� � ��������������
; ��������� ������.
; 
; ��������� ������ ����������� �����������,  ��� ������������ �
; ���������������� ��������� �� ������������ � ��������� �������������,
; ���������� ����. 
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
