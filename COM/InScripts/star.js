var ms = ThisDrawing.ModelSpace;
var ut  = ThisDrawing.Utility;
var ptCenter = ut.GetPoint(null, "Укажите центр звезды");
var n;

while ((n = ut.GetInteger("Укажите количество лучей (больше 2х)")) < 3)
  ;

var d = ut.GetDistance(ptCenter, "Задайте внутренний радиус")
var D = ut.GetDistance(ptCenter, "Задайте внешний радиус")
var da = Math.PI * 2 / n;
var hd = da / 2;

for (i = 0; i < n; ++i)  {
  var a = da * i;
  var pt1 = ut.PolarPoint(ptCenter, a - hd, d)
  var pt2 = ut.PolarPoint(ptCenter, a, D)
  var pt3 = ut.PolarPoint(ptCenter, a + hd, d)
  ms.AddLine(pt1, pt2);
  ms.AddLine(pt2, pt3);
}

function ptdump(ut, pt)
{
  var sp = new VBArray(ut.CreateSafeArrayFromVector(pt))
  return  "(" + sp.toArray() + ")";
}

