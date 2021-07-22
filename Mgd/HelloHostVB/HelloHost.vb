'
' Копирайт (С) 2016, ЗАО «Нанософт». Все права защищены.
' 
' Данное программное обеспечение, все исключительные права на него, его
' документация и сопроводительные материалы принадлежат ЗАО «Нанософт».
' Данное программное обеспечение может использоваться при разработке и входить
' в состав разработанных программных продуктов при соблюдении условий
' использования, оговоренных в «Лицензионном договоре присоединения
' на использование программы для ЭВМ «Платформа nanoCAD»».
' 
' Данное программное обеспечение защищено в соответствии с законодательством
' Российской Федерации об интеллектуальной собственности и международными
' правовыми актами.
' 
' Используя данное программное обеспечение,  его документацию и
' сопроводительные материалы вы соглашаетесь с условиями использования,
' указанными выше. 
'
#If NCAD Then
Imports HostMgd.ApplicationServices
Imports HostMgd.EditorInput
Imports Teigha.Runtime
Imports Teigha.DatabaseServices
Imports HostMgd.Runtime
Imports Platform = HostMgd
Imports PlatformDb = Teigha
#Else
Imports Autodesk.AutoCAD.ApplicationServices
Imports Autodesk.AutoCAD.DatabaseServices
Imports Autodesk.AutoCAD.EditorInput
Imports Autodesk.AutoCAD.Geometry
Imports Autodesk.AutoCAD.Runtime
Imports Platform = Autodesk.AutoCAD
Imports PlatformDb = Autodesk.AutoCAD
#End If


Namespace HelloHostVB
  Public Class HelloHost
    ' <summary>
    ' Работа с командной строкой
    ' </summary>
    <CommandMethod("HelloHost_Example1_VB")> Public Sub Template1()

      Dim Ed As Editor = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor

      ' Выводим в командную строку сообщение
      Ed.WriteMessage("Добро пожаловать в управляемый код nanoCAD!")

      Dim opts As PromptStringOptions = New PromptStringOptions("Введите строку")
      opts.AllowSpaces = True
      Dim pr As PromptResult = Ed.GetString(opts)
      If (PromptStatus.OK = pr.Status) Then
        Ed.WriteMessage("Вы ввели: " + pr.StringResult)
      Else
        Ed.WriteMessage("Отмена.")
      End If



      Dim kwopts As PromptKeywordOptions = New PromptKeywordOptions("Укажите ключевое слово")
      kwopts.AllowNone = False
      kwopts.Keywords.Add("One")
      kwopts.Keywords.Add("Two")
      kwopts.Keywords.Add("Three")

      kwopts.AppendKeywordsToMessage = True

      Dim kw As PromptResult = Ed.GetKeywords(kwopts)
      If (kw.Status = PromptStatus.OK) Then
        Ed.WriteMessage("Вы ввели правильное ключевое слово. Вы ввели: " + kw.StringResult)
      End If
    End Sub

    ' <summary>
    ' Работа со слоями
    ' </summary>
    <CommandMethod("HelloHost_Example2_VB")> Public Sub Template2()
      Dim db As Database = HostApplicationServices.WorkingDatabase
      Dim doc As Document = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument
      Dim ed As Editor = doc.Editor
      Dim tm As PlatformDb.DatabaseServices.TransactionManager = db.TransactionManager

      ' Выводим в командную строку информацию о первых 10 слоях
      ed.WriteMessage("Выводим первые 10 слоев:")
      Using myT As Transaction = tm.StartTransaction()
        Dim lt As LayerTable = CType(tm.GetObject(db.LayerTableId, OpenMode.ForRead, False), LayerTable)
        Dim ltrec As LayerTableRecord

        Dim lte As SymbolTableEnumerator = lt.GetEnumerator()
        For i As Integer = 0 To 9
          If Not lte.MoveNext() Then
            Exit For
          End If

          Dim id As ObjectId = CType(lte.Current, ObjectId)
          ltrec = CType(tm.GetObject(id, OpenMode.ForRead), LayerTableRecord)
          ed.WriteMessage(String.Format("Имя слоя:{0}; Цвет слоя: {1}; Код слоя:{2}", ltrec.Name, ltrec.Color.ToString(), ltrec.Description))
        Next
      End Using
      Dim opts As PromptStringOptions = New PromptStringOptions("Введите имя нового слоя")
      opts.AllowSpaces = True
      Dim pr As PromptResult = ed.GetString(opts)
      If PromptStatus.OK = pr.Status Then
        Dim newLayerName As String = pr.StringResult
        ' Создаем новый слой
        Using myT As Transaction = tm.StartTransaction()
          Try
            Dim lt As LayerTable = CType(tm.GetObject(db.LayerTableId, OpenMode.ForWrite, False), LayerTable)
            'Проверяем есть ли такой слой
            If Not (lt.Has(newLayerName)) Then
              Dim ltrec As LayerTableRecord = New LayerTableRecord()
              ltrec.Name = newLayerName
              lt.Add(ltrec)
              tm.AddNewlyCreatedDBObject(ltrec, True)
              myT.Commit()
            End If
          Finally
            myT.Dispose()
          End Try
        End Using
      Else
        ed.WriteMessage("Отмена.")
      End If
    End Sub
  End Class
End Namespace

