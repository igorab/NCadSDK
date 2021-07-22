Dim ms, ut
Dim n, d, dt, da, dh, a
Dim ptCenter, pt1, pt2, pt3, pi, e

appName = "nanoCAD.Application"

On Error Resume Next

Set app = GetObject("", appName)
If Err <> 0 Then
  Set app = CreateObject(appName)
  app.Visible = True
  echo "create it"
else
  echo "get it"
End If

On Error Goto 0

Set doc = app.ActiveDocument

Set ms = doc.ModelSpace
Set ut  = doc.Utility
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
