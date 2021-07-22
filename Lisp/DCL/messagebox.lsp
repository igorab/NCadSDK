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

; shows message box from .dcl file
(defun show-message-box (title msg)
  ; load DCL file
  (setq dcl (load_dialog "messagebox.dcl"))
  ; create new messagebox dialog
  (new_dialog "messagebox" dcl)
  ; set title and contents of messagebox
  (set_tile "dlg" title)
  (set_tile "message" msg)
  ; start dialog
  (setq res (start_dialog))
  ; after dialog exits, unload DCL file
  (unload_dialog dcl)
  res)

; message box command
(defun c:show-message-box ()
  (show-message-box "Message" "Hello!"))
