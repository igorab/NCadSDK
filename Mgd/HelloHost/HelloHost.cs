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

namespace HelloHost
{
  using System;
  using System.Collections.Generic;
  using System.Linq;
  using System.Text;

#if NCAD
  using Teigha.DatabaseServices;
  using Teigha.Runtime;
  using Teigha.Geometry;
  using HostMgd.ApplicationServices;
  using HostMgd.EditorInput;

  using Platform = HostMgd;
  using PlatformDb = Teigha;
#else
  using Autodesk.AutoCAD.ApplicationServices;
  using Autodesk.AutoCAD.DatabaseServices;
  using Autodesk.AutoCAD.EditorInput;
  using Autodesk.AutoCAD.Geometry;
  using Autodesk.AutoCAD.Runtime;
  using AcadApp = Autodesk.AutoCAD.ApplicationServices.Application;

  using Platform = Autodesk.AutoCAD;
  using PlatformDb = Autodesk.AutoCAD;
#endif

  /// <summary>
  /// Главный класс сборки
  /// </summary>
  public class HelloHost
  {
    /// <summary>
    /// Работа с командной строкой
    /// </summary>
    [CommandMethod("HelloHost_Example1")]
    public void Template1()
    {
      Editor ed = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;

      // Выводим в командную строку сообщение
      ed.WriteMessage("Добро пожаловать в управляемый код nanoCAD!");

      PromptStringOptions opts = new PromptStringOptions("Введите строку");
      opts.AllowSpaces = true;
      PromptResult pr = ed.GetString(opts);
      if (PromptStatus.OK == pr.Status)
      {
        ed.WriteMessage("Вы ввели: " + pr.StringResult);
      }
      else
      {
        ed.WriteMessage("Отмена.");
      }

      PromptKeywordOptions kwopts = new PromptKeywordOptions("Укажите ключевое слово");
      kwopts.AllowNone = false;
      kwopts.Keywords.Add("Первое");
      kwopts.Keywords.Add("Второе");
      kwopts.Keywords.Add("Еще_одно");

      kwopts.AppendKeywordsToMessage = true;

      PromptResult kw = ed.GetKeywords(kwopts);
      if (PromptStatus.Keyword == kw.Status)
      {
        ed.WriteMessage("Вы ввели правильное ключевое слово. Вы ввели: " + kw.StringResult);
      }
    }

    /// <summary>
    /// Работа со слоями
    /// </summary>
    [CommandMethod("HelloHost_Example2")]
    public void Template2()
    {
      Database db = HostApplicationServices.WorkingDatabase;
      Document doc = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
      Editor ed = doc.Editor;
      PlatformDb.DatabaseServices.TransactionManager tm = db.TransactionManager;

      // Выводим в командную строку информацию о первых 10 слоях
      ed.WriteMessage("Выводим первые 10 слоев:");
      using (Transaction myT = tm.StartTransaction())
      {
        LayerTable lt = (LayerTable)tm.GetObject(db.LayerTableId, OpenMode.ForRead, false);
        LayerTableRecord ltrec;

        SymbolTableEnumerator lte = lt.GetEnumerator();
        for (int i = 0; i < 10; ++i)
        {
          if (!lte.MoveNext())
          {
            break;
          }

          ObjectId id = (ObjectId)lte.Current;
          ltrec = (LayerTableRecord)tm.GetObject(id, OpenMode.ForRead);
          ed.WriteMessage(string.Format("Имя слоя:{0}; Цвет слоя: {1}; Код слоя:{2}", ltrec.Name, ltrec.Color.ToString(), ltrec.Description));
        }
      }
      
      PromptStringOptions opts = new PromptStringOptions("Введите имя нового слоя");
      opts.AllowSpaces = true;
      PromptResult pr = ed.GetString(opts);
      if (PromptStatus.OK == pr.Status)
      {
        string newLayerName = pr.StringResult;

        // Создаем новый слой
        using (Transaction myT = tm.StartTransaction())
        {
          try
          {
            LayerTable lt = (LayerTable)tm.GetObject(db.LayerTableId, OpenMode.ForWrite, false);

            // Проверяем есть ли такой слой
            if (!lt.Has(newLayerName))
            {
              LayerTableRecord ltrec = new LayerTableRecord();
              ltrec.Name = newLayerName;
              lt.Add(ltrec);
              tm.AddNewlyCreatedDBObject(ltrec, true);
              myT.Commit();
            }
          }
          finally
          {
            myT.Dispose();
          }
        }
      }
      else
      {
        ed.WriteMessage("Отмена.");
      }
    }
  }
}