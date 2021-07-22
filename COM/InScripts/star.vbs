Dim ms, ut
Dim n, d, dt, da, dh, a
Dim ptCenter, pt1, pt2, pt3, pi, e

Set ms = ThisDrawing.ModelSpace
Set ut  = ThisDrawing.Utility
ptCenter = ut.GetPoint("0,0,0", "Укажите центр звезды")
Do
  n = ut.GetInteger("Укажите количество лучей (больше 2х)")
Loop While n <= 2

pi = 3.14159
d = ut.GetDistance(ptCenter, "Задайте внутренний радиус")
dt = ut.GetDistance(ptCenter, "Задайте внешний радиус")
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
