Dim ms, ut
Dim n, d, dt, da, dh, a
Dim ptCenter, pt1, pt2, pt3, pi, e

Set ms = ThisDrawing.ModelSpace
Set ut  = ThisDrawing.Utility
ptCenter = ut.GetPoint("0,0,0", "������� ����� ������")
Do
  n = ut.GetInteger("������� ���������� ����� (������ 2�)")
Loop While n <= 2

pi = 3.14159
d = ut.GetDistance(ptCenter, "������� ���������� ������")
dt = ut.GetDistance(ptCenter, "������� ������� ������")
da = (pi * 2) / n
hd = da / 2
 
For i = 0 to n  
  a = da * i
  pt1 = ut.PolarPoint(ptCenter, a - hd, d)
  pt2 = ut.PolarPoint(ptCenter, a, dt)
  pt3 = ut.PolarPoint(ptCenter, a + hd, d)
  ms.AddLine pt1, pt2
  ms.AddLine pt2, pt3
Next
