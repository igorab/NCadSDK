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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

#if NCAD
using Teigha.DatabaseServices;
using Teigha.Runtime;
using Teigha.Geometry;
using HostMgd.ApplicationServices;
using HostMgd.EditorInput;

using Platform = HostMgd;
using NativePlatform = Teigha;
#else
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;
using AcadApp = Autodesk.AutoCAD.ApplicationServices.Application;

using Platform = Autodesk.AutoCAD;
using NativePlatform = Autodesk.AutoCAD;
#endif

using HostMgd.Samples;

namespace LstCrCircles
{

  public class LstCrCircles : System.Windows.Forms.UserControl
  {
    /// <summary> 
    /// Required designer variable.
    /// </summary>
    private System.Windows.Forms.Label label1;
    private ListView listView1;
    private ColumnHeader columnHeader1;
    private ColumnHeader columnHeader2;
    private ColumnHeader columnHeader3;
    
    private bool bSelectAction = true;
    
    private System.ComponentModel.Container components = null;

    public LstCrCircles()
    {
      InitializeComponent();

      DocumentCollection dm = Platform.ApplicationServices.Application.DocumentManager;

      this.label1.Text = dm.MdiActiveDocument.Window.Text;

      dm.DocumentCreated += new DocumentCollectionEventHandler(OnDocumentCreated);
      dm.DocumentToBeDestroyed += new DocumentCollectionEventHandler(OnDocumentToBeDestroyed);
      dm.DocumentActivated += new DocumentCollectionEventHandler(OnDocumentActivated);

      Platform.ApplicationServices.Application.SystemVariableChanged +=
            new Platform.ApplicationServices.SystemVariableChangedEventHandler(appSysVarChanged);

      foreach (Document doc in dm)
      {
        doc.Database.ObjectAppended += new ObjectEventHandler(callback_ObjectAppended);
        doc.Database.ObjectErased += new ObjectErasedEventHandler(callback_ObjectErased);
        doc.Database.ObjectModified += new ObjectEventHandler(callback_ObjectModified);
      }

      PolulateList(dm.MdiActiveDocument);
    }

    /// <summary> 
    /// Clean up any resources being used.
    /// </summary>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Component Designer generated code
    /// <summary> 
    /// Required method for Designer support - do not modify 
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.label1 = new System.Windows.Forms.Label();
      this.listView1 = new System.Windows.Forms.ListView();
      this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
      this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
      this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
      this.SuspendLayout();
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(11, 7);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(52, 15);
      this.label1.TabIndex = 8;
      this.label1.Text = "Чертеж";
      // 
      // listView1
      // 
      this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
      this.listView1.FullRowSelect = true;
      this.listView1.HideSelection = false;
      this.listView1.Location = new System.Drawing.Point(6, 29);
      this.listView1.Name = "listView1";
      this.listView1.Size = new System.Drawing.Size(229, 263);
      this.listView1.TabIndex = 9;
      this.listView1.UseCompatibleStateImageBehavior = false;
      this.listView1.View = System.Windows.Forms.View.Details;
      this.listView1.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged);
      // 
      // columnHeader1
      // 
      this.columnHeader1.Text = "Handle";
      // 
      // columnHeader2
      // 
      this.columnHeader2.Text = "X";
      this.columnHeader2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      this.columnHeader2.Width = 80;
      // 
      // columnHeader3
      // 
      this.columnHeader3.Text = "Y";
      this.columnHeader3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      this.columnHeader3.Width = 80;
      // 
      // LstCrCircles
      // 
      this.Controls.Add(this.listView1);
      this.Controls.Add(this.label1);
      this.Name = "LstCrCircles";
      this.Size = new System.Drawing.Size(241, 296);

      this.ResumeLayout(false);
      this.PerformLayout();

    }
    #endregion

    private void OnDocumentCreated(object sender, DocumentCollectionEventArgs e)
    {
      e.Document.Database.ObjectAppended += new ObjectEventHandler(callback_ObjectAppended);
      e.Document.Database.ObjectErased += new ObjectErasedEventHandler(callback_ObjectErased);
      e.Document.Database.ObjectModified += new ObjectEventHandler(callback_ObjectModified);
    }

    public void appSysVarChanged(object sender, 
      Platform.ApplicationServices.SystemVariableChangedEventArgs sysVarChEvtArgs)
    {
      if (sysVarChEvtArgs.Changed && (sysVarChEvtArgs.Name == "UCSNAME"))
      {
        Document acDoc = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
        Editor ed = acDoc.Editor;

        PolulateList(ed.Document);
      }
    }
    
    private void OnDocumentActivated(object sender, DocumentCollectionEventArgs e)
    {
      this.label1.Text = e.Document.Window.Text;
      PolulateList(e.Document);
    }

    private void OnDocumentToBeDestroyed(object sender, DocumentCollectionEventArgs e)
    {
      this.label1.Text = string.Empty;
      listView1.Items.Clear();
      e.Document.Database.ObjectAppended -= new ObjectEventHandler(callback_ObjectAppended);
      e.Document.Database.ObjectErased -= new ObjectErasedEventHandler(callback_ObjectErased);
      e.Document.Database.ObjectModified -= new ObjectEventHandler(callback_ObjectModified);
    }

    private void PolulateList(Document acDoc)
    {
      Database acCurDb = acDoc.Database;

      // Заполним список 

      using (Transaction acTrans = acCurDb.TransactionManager.StartTransaction())
      {
        BlockTable acBlkTbl;
        acBlkTbl = acTrans.GetObject(acCurDb.BlockTableId, 
                                     OpenMode.ForRead) as BlockTable;

        BlockTableRecord acBlkTblRec;
        acBlkTblRec = acTrans.GetObject(acBlkTbl[BlockTableRecord.ModelSpace],
                                        OpenMode.ForRead) as BlockTableRecord;
        listView1.Items.Clear();
        listView1.BeginUpdate();

        foreach (ObjectId asObjId in acBlkTblRec)
        {
          DBObject dbObj = acTrans.GetObject(asObjId, OpenMode.ForRead);
          HostMgd.Samples.MgCrCrc dbObjCC = dbObj as HostMgd.Samples.MgCrCrc;
          if (dbObjCC != null)
          {
            addObjectToList(dbObjCC);
          }
        }

        listView1.EndUpdate();

        acTrans.Commit();
      }
      SelObToItems(acDoc);
    }

    private void SelObToItems(Document acDoc)
    {
      Database acCurDb = acDoc.Database;
      Editor ed = acDoc.Editor;

      PromptSelectionResult sr = ed.SelectImplied();
      if (sr.Status == PromptStatus.OK)
      {
        Transaction tr = acCurDb.TransactionManager.StartTransaction();

        try
        {
          ObjectId[] objIds = sr.Value.GetObjectIds();

          bSelectAction = false;

          foreach (ObjectId asObjId in objIds)
          {

            if (asObjId.ObjectClass.Name == "AcDbCrossCircle")
            {
              DBObject obj = tr.GetObject(asObjId, OpenMode.ForRead);
              Entity ent = (Entity)obj;

              // Выделим в списке

              ListViewItem li = listView1.FindItemWithText(ent.Handle.ToString());

              if (li != null)
              {
                if (li.Selected != true)
                  li.Selected = true;
              }

              obj.Dispose();
            }
          }

          bSelectAction = true;

          tr.Commit();
        }

        catch (NativePlatform.Runtime.Exception ex)
        {
          ed.WriteMessage(ex.Message);
          tr.Abort();
        }
      }
      else
      {
        ListView.SelectedListViewItemCollection selItems = listView1.SelectedItems;

        foreach (ListViewItem item in selItems)
        {
          item.Selected = false;
        }
      }
    }

    private void listView1_SelectedIndexChanged(object sender, EventArgs e)
    {
      if (bSelectAction)
      {
        Document doc = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
        Editor ed = doc.Editor;

        ListView.SelectedListViewItemCollection selItems = listView1.SelectedItems;

        List<ObjectId> objects = new List<ObjectId>();
        foreach (ListViewItem item in selItems)
        {
          objects.Add((ObjectId)item.Tag);
        }
        ed.SetImpliedSelection(objects.ToArray());
        Platform.ApplicationServices.Application.UpdateScreen();
      }
    }

    private void callback_ObjectAppended(object sender, ObjectEventArgs e)
    {
      HostMgd.Samples.MgCrCrc dbObjCC = e.DBObject as HostMgd.Samples.MgCrCrc;
      if (dbObjCC == null)
      {
        return;
      }

      if (dbObjCC.OwnerId == dbObjCC.Database.CurrentSpaceId)
      {
        addObjectToList(dbObjCC);
      }
    }

    private void callback_ObjectErased(object sender, ObjectErasedEventArgs e)
    {
      HostMgd.Samples.MgCrCrc dbObjCC = e.DBObject as HostMgd.Samples.MgCrCrc;
      if (dbObjCC == null)
      {
        return;
      }

      // если объект был стерт
      if (e.Erased)
      {
        ListViewItem item = listView1.FindItemWithText(dbObjCC.Handle.ToString());
        if (item != null)
        {
          item.Remove();
        }
      }
      else
      {
        if (dbObjCC.OwnerId == dbObjCC.Database.CurrentSpaceId)
        {
          addObjectToList(dbObjCC);
        }
      }
    }

    private void callback_ObjectModified(object sender, ObjectEventArgs e)
    {
      HostMgd.Samples.MgCrCrc dbObjCC = e.DBObject as HostMgd.Samples.MgCrCrc;
      if (dbObjCC == null)
      {
        return;
      }

      if (dbObjCC.OwnerId == dbObjCC.Database.CurrentSpaceId && (dbObjCC.IsModified))
      {
        modObjectInList(dbObjCC);
      }
    }

    private void addObjectToList(HostMgd.Samples.MgCrCrc dbObjCC)
    {
      Editor ed = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
      Matrix3d matUCS = ed.CurrentUserCoordinateSystem;

      Point3d Cent = dbObjCC.mycenter.TransformBy(matUCS.Inverse());

      ObjectId objId = dbObjCC.ObjectId;
      ListViewItem item = new ListViewItem(objId.Handle.ToString());

      item.Tag = objId;
      item.SubItems.Add(Platform.Runtime.Converter.DistanceToString(Cent.X));
      item.SubItems.Add(Platform.Runtime.Converter.DistanceToString(Cent.Y));
      listView1.Items.Add(item);
    }

    private void modObjectInList(HostMgd.Samples.MgCrCrc dbObjCC)
    {
      Editor ed = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
      Matrix3d matUCS = ed.CurrentUserCoordinateSystem;

      Point3d Cent = dbObjCC.mycenter.TransformBy(matUCS.Inverse());

      ListViewItem item = listView1.FindItemWithText(dbObjCC.ObjectId.Handle.ToString());

      if (item != null)
      {
        item.SubItems[1].Text = Platform.Runtime.Converter.DistanceToString(Cent.X);
        item.SubItems[2].Text = Platform.Runtime.Converter.DistanceToString(Cent.Y);
      }
    }
  }

  public class ListPalettes
  {
    static Platform.Windows.PaletteSet ps;
    [CommandMethod("ListCrCr", CommandFlags.UsePickSet | CommandFlags.Redraw)]
    public static void ListCrCr()
    {
      if (ps == null)
      {
        //use constructor with Guid so that we can save/load user data
        ps = new Platform.Windows.PaletteSet("Список AcDbCrossCircle", "listcrcr", new Guid("e88d0f3a-7ae7-4010-9bc9-220e0aba75d6"));
        ps.MinimumSize = new System.Drawing.Size(241, 300);
        ps.Size = new System.Drawing.Size(241, 300);
        ps.Add("Список AcDbCrossCircle", new LstCrCircles());
      }

      ps.Visible = true;
    }

  }

  public class TestEntityJig
  {
    class CrossCircleJig : EntityJig
    {
      Point3d mCenterPt;
      Vector3d mNormal, mDirection;

      int mPromptCounter;

      public CrossCircleJig(Vector3d vec)
        : base(new MgCrCrc())
      {
        mCenterPt = new Point3d();
        mNormal = vec;
        mDirection = new Vector3d();
        mPromptCounter = 0;
      }

      protected override SamplerStatus Sampler(JigPrompts prompts)
      {
        JigPromptPointOptions jigOpts = new JigPromptPointOptions();
        jigOpts.UserInputControls = (UserInputControls.Accept3dCoordinates | UserInputControls.NoZeroResponseAccepted
          | UserInputControls.NoNegativeResponseAccepted);

        if (mPromptCounter == 0)
        {
          jigOpts.Message = "\nЦентр круга: ";
          PromptPointResult dres = prompts.AcquirePoint(jigOpts);

          if (dres.Status == PromptStatus.OK)
          {
            mCenterPt = dres.Value;
            return SamplerStatus.OK;
          }
          else
          {
            return SamplerStatus.Cancel;
          }
        }
        else if (mPromptCounter == 1)
        {
          jigOpts.BasePoint = mCenterPt;
          jigOpts.UseBasePoint = true;
          jigOpts.Message = "\nРадиус круга: ";
          PromptPointResult res = prompts.AcquirePoint(jigOpts);

          Vector3d acqVector = res.Value - mCenterPt;
          Vector3d directionTemp = acqVector.Length * acqVector.OrthoProjectTo(mNormal).GetNormal();

          if (directionTemp != mDirection)
            mDirection = directionTemp;
          else
            return SamplerStatus.NoChange;

          if (res.Status == PromptStatus.Cancel)
            return SamplerStatus.Cancel;
          else
            return SamplerStatus.OK;

        }
        else
        {
          return SamplerStatus.NoChange;
        }


      }
      protected override bool Update()
      {
        try
        {
          ((MgCrCrc)Entity).mynormal = mNormal;
          ((MgCrCrc)Entity).mycenter = mCenterPt;
          ((MgCrCrc)Entity).mydirection = mDirection;
        }
        catch (System.Exception)
        {
          return false;
        }

        return true;

      }

      public void setPromptCounter(int i)
      {
        mPromptCounter = i;
      }
      public Entity GetEntity()
      {
        return Entity;
      }

    }
    [CommandMethod("ccjig")]
    static public void DoIt()
    {
      Document curDoc = Platform.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
      Editor ed = curDoc.Editor;

      Database db = curDoc.Database;

      Vector3d x = db.Ucsxdir;
      Vector3d y = db.Ucsydir;
      Vector3d NormalVec = x.CrossProduct(y);

      //Create CrossCircleJig
      CrossCircleJig jig = new CrossCircleJig(NormalVec.GetNormal());
      //first call drag to get the center
      jig.setPromptCounter(0);
      ed.Drag(jig);
      // Again call drag to get direction
      jig.setPromptCounter(1);
      ed.Drag(jig);

      //Append entity.
      Transaction tr = db.TransactionManager.StartTransaction();
      using (tr)
      {
        BlockTable bt = (BlockTable)tr.GetObject(db.BlockTableId, OpenMode.ForRead, false);
        BlockTableRecord btr = (BlockTableRecord)tr.GetObject(bt[BlockTableRecord.ModelSpace], OpenMode.ForWrite, false);
        btr.AppendEntity(jig.GetEntity());
        tr.AddNewlyCreatedDBObject(jig.GetEntity(), true);
        tr.Commit();
      }

    }

  }

}