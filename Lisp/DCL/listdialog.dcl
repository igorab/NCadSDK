//
// �������� (�) 2019, ��� ��������� ����������. ��� ����� ��������.
// 
// ������ ����������� �����������, ��� �������������� ����� �� ����, ���
// ������������ � ���������������� ��������� ����������� ��� ��������� ����������.
// ������ ����������� ����������� ����� �������������� ��� ���������� � �������
// � ������ ������������� ����������� ��������� ��� ���������� �������
// �������������, ����������� � ������������� �������� �������������
// �� ������������� ��������� ��� ��� ���������� nanoCAD��.
// 
// ������ ����������� ����������� �������� � ������������ � �����������������
// ���������� ��������� �� ���������������� ������������� � ��������������
// ��������� ������.
// 
// ��������� ������ ����������� �����������,  ��� ������������ �
// ���������������� ��������� �� ������������ � ��������� �������������,
// ���������� ����. 
//

listdialog : dialog {
    label = "List dialog";
    :column {
        :row {
            :edit_box {
                key = "edit";
                label = "Input item: ";
                edit_width = 20;
            }
            :button {
                key = "add";
                label = "Add";
            }
            :button {
                key = "clear";
                label = "Clear";
            }
        }
        :list_box {
            key = "list";
        }
        ok_cancel;
    }
}
