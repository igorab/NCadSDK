//
// Копирайт (С) 2019, ООО «Нанософт разработка». Все права защищены.
// 
// Данное программное обеспечение, все исключительные права на него, его
// документация и сопроводительные материалы принадлежат ООО «Нанософт разработка».
// Данное программное обеспечение может использоваться при разработке и входить
// в состав разработанных программных продуктов при соблюдении условий
// использования, оговоренных в «Лицензионном договоре присоединения
// на использование программы для ЭВМ «Платформа nanoCAD»».
// 
// Данное программное обеспечение защищено в соответствии с законодательством
// Российской Федерации об интеллектуальной собственности и международными
// правовыми актами.
// 
// Используя данное программное обеспечение,  его документацию и
// сопроводительные материалы вы соглашаетесь с условиями использования,
// указанными выше. 
//

using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using System.ComponentModel;
using Multicad;
using Multicad.AplicationServices;
using Multicad.Runtime;
using Multicad.DatabaseServices;
using Multicad.DataServices;
using Multicad.Geometry;
using Multicad.CustomObjectBase;


namespace Multicad.Samples
{
	[CustomEntity("1C925FA1-842B-49CD-924F-4ABF9717DB62", "TextInBox", "TextInBox Sample Entity")]
	[Serializable]
	public class TextInBox : McCustomBase
	{
		private Point3d _pnt1 = new Point3d(50, 50, 0);
		private Point3d _pnt2 = new Point3d(150, 100, 0);
		private String _text = "Text field";
		public TextInBox()
		{
		}
		[DisplayName("Текстовая метка")]
		[Description("Описание метки")]
		[Category("Тестовый объект")]
		public String Text
		{
			get
			{
				return _text;
			}
			set
			{
				if (!TryModify()) return;//без этого не будет сохранятся Undo и перерисовыватся объект
				_text = value;
			}
		}
		public override void OnDraw(GeometryBuilder dc)
		{
			dc.Clear();
			dc.Color = McDbEntity.ByObject;//цвет будет братся из свойств объекта, и при изменении автоматически перерисуется
			dc.DrawPolyline(new Point3d[] { _pnt1, new Point3d(_pnt1.X, _pnt2.Y, 0), _pnt2, new Point3d(_pnt2.X, _pnt1.Y, 0), _pnt1});
			dc.TextHeight = 2.5 * DbEntity.Scale;	//Используем масштаб оформления
			dc.Color = Color.Blue;//Текст рисуем синим цветом
			dc.DrawMText(new Point3d((_pnt2.X + _pnt1.X) / 2.0, (_pnt2.Y + _pnt1.Y) / 2.0, 0), Vector3d.XAxis, Text, HorizTextAlign.Center, VertTextAlign.Center);
		}
		public override void OnTransform(Matrix3d tfm)
		{
			if (!TryModify()) return;
			_pnt1 = _pnt1.TransformBy(tfm);
			_pnt2 = _pnt2.TransformBy(tfm);
		}
		public override List<Point3d> OnGetGripPoints()
		{
			List<Point3d> arr = new List<Point3d>();
			arr.Add(_pnt1);
			arr.Add(_pnt2);
			return arr;
		}
		public override void OnMoveGripPoints(List<int> indexes, Vector3d offset, bool isStretch)
		{
			if (!TryModify()) return;
			if (indexes.Count == 2)
			{
				_pnt1 += offset;
				_pnt2 += offset;
			}
			else if (indexes.Count == 1)
			{
				if (indexes[0] == 0)
				{
					_pnt1 += offset;
				}
				else
				{
					_pnt2 += offset;
				}
			}
		}

		public override hresult PlaceObject(PlaceFlags lInsertType)
		{
			InputJig jig = new InputJig();
			InputResult res = jig.GetPoint("Select first point:");
			if (res.Result != InputResult.ResultCode.Normal)
				return hresult.e_Fail;
			_pnt1 = res.Point;
			DbEntity.AddToCurrentDocument();
			//Исключаем себя из привязки, что бы osnap точки не липли к самому себе
			jig.ExcludeObject(ID);
			//Мониторинг движения мышкой
			jig.MouseMove = (s, a) => { TryModify(); _pnt2 = a.Point; DbEntity.Update(); };

			res = jig.GetPoint("Select second point:");
			if (res.Result != InputResult.ResultCode.Normal)
			{
				DbEntity.Erase();
				return hresult.e_Fail;
			}
			_pnt2 = res.Point;
			return hresult.s_Ok;
		}
		public override hresult OnEdit(Point3d pnt, EditFlags lInsertType)
		{
			TextInBox_Form frm = new TextInBox_Form();
			frm.textBox1.Text = Text;
			frm.ShowDialog();
			Text = frm.textBox1.Text;
			return hresult.s_Ok;
		}
	}
}
