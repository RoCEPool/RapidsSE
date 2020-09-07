/*============================================================================*/
/* Kbase.cpp - Kbase                                                          */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Kbase,CN_KBASE)
__aNewFunction(Kbase,CN_KBASE)
__vDeleteFunction(Kbase,CN_KBASE)
__aFooterFunction(Kbase)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Kbase::Kbase(Stz* szKbName)
: Object(CN_KBASE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  vAprioriFacts();
 
  _checkInitialised_(CN_KBASE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Kbase::~Kbase(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pConceptAdd(Fact* pSuper, Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = pSuper->pNewInstance(szNote);
  
  m_p_Concept_->vElementAdd(pConcept);

  return pConcept;
}

/*----------------------------------------------------------------------------*/
/* get root fact                                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  return m_pFact;
}

/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFactNew(Fact* pSuper, Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFact = new Fact(this, pSuper, szNote);
  
  return pFact;
}

/*----------------------------------------------------------------------------*/
/* delete a fact                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vFactDelete(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete pFact;
}

/*----------------------------------------------------------------------------*/
/* find element                                                               */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFindElement(Stz* szNote, Fact* pSuper)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pElement = m_pContents->pFindElement(szNote,pSuper);

  return pElement;
}


/*----------------------------------------------------------------------------*/
/* generate next fact no                                                      */
/*----------------------------------------------------------------------------*/
i8 Kbase::iNextFactNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  i8 iFactNo = ++ m_ixFactNo;
  
  return iFactNo;
}

/*----------------------------------------------------------------------------*/
/* get property fact                                                          */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pProperty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  return m_p_Property_;
}

/*----------------------------------------------------------------------------*/
/* borrow/new a question                                                      */
/*----------------------------------------------------------------------------*/
Question* Kbase::pQuestionBorrow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Question* pQuestion = new Question(this);
  
  return pQuestion;
}

/*----------------------------------------------------------------------------*/
/* return/delete a question                                                   */
/*----------------------------------------------------------------------------*/
void Kbase::vQuestionReturn(Question** ppQuestion)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete *ppQuestion;
  
  *ppQuestion = nullptr;
}

/*----------------------------------------------------------------------------*/
/* borrow/new a transform                                                     */
/*----------------------------------------------------------------------------*/
Transform* Kbase::pTransformBorrow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Transform* pTransform = new Transform(this);
  
  return pTransform;
}

/*----------------------------------------------------------------------------*/
/* return/delete a transform                                                  */
/*----------------------------------------------------------------------------*/
void Kbase::vTransformReturn(Transform** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete *ppTransform;
  
  *ppTransform = nullptr;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* import state                                                               */
/*----------------------------------------------------------------------------*/
void Kbase::vImportStatement(Stz* szStmtToImport)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  
}

/*----------------------------------------------------------------------------*/
/* import table                                                               */
/*----------------------------------------------------------------------------*/
void Kbase::vImportTable(Table* pTableToImport)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  vTrace("Import %s ...\n"_Stz,pTableToImport->szTableNm());
  
  Fact* pTables  = m_pContents->pFindElement("tables"_Stz);
  Fact* p_Table_       = m_p_Concept_->pFindElement("_table_"_Stz);
  Fact* p_TableNm_     = m_p_Concept_->pFindElement("_tableNm_"_Stz);
  Fact* p_Columns_     = m_p_Concept_->pFindElement("_columns_"_Stz);
  Fact* p_Column_      = m_p_Concept_->pFindElement("_column_"_Stz);
  Fact* p_ColumnNm_    = m_p_Concept_->pFindElement("_columnNm_"_Stz);
  Fact* p_ColumnType_  = m_p_Concept_->pFindElement("_columnType_"_Stz);
  Fact* p_Length_      = m_p_Concept_->pFindElement("_length_"_Stz);
  Fact* p_Scale_       = m_p_Concept_->pFindElement("_scale_"_Stz);
  Fact* p_Nullable_    = m_p_Concept_->pFindElement("_nullable_"_Stz);
  Fact* p_NotNullable_ = m_p_Concept_->pFindElement("_notNullable_"_Stz);
  Fact* p_PtnKey_      = m_p_Concept_->pFindElement("_ptnKey_"_Stz);

  Fact* pTable = p_Table_->pNewInstance();
  pTables->vElementAdd(pTable);

  Fact* pTableNm = p_TableNm_->pNewInstance();
  pTableNm->rxProperty().m_pText = new Text;
  pTableNm->rxProperty().m_pText->vSet(pTableToImport->szTableNm());
  pTable->vAttributeAdd(pTableNm);

  Fact* pColumns = p_Columns_->pNewInstance();
  pTable->vElementAdd(pColumns);

  StreamDef* pStmDef = pTableToImport->pStreamDef();
  
  i8 nCols = pStmDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pCol = pStmDef->pGetColumn(i_Col);
    
    Fact* pColumn = p_Column_->pNewInstance();
    pColumns->vElementAdd(pColumn);
    
    Fact* pColumnNm = p_ColumnNm_->pNewInstance();
    pColumnNm->rxProperty().m_pText = new Text;
    pColumnNm->rxProperty().m_pText->vSet(pCol->szColNm());
    pColumn->vAttributeAdd(pColumnNm);

    eCT eColType = pCol->eColumnType();
    Fact* p_ColType_ = m_p_Concept_->pFindElement(aCT[eColType],p_ColumnType_);
    Fact* pColType = p_ColType_->pNewInstance();
    pColumn->vAttributeAdd(pColType);
    
    i8 iLength = pCol->iColumnLength();
    Fact* pLength = p_Length_->pNewInstance();
    pLength->rxProperty().m_iInt = iLength;
    pColumn->vAttributeAdd(pLength);

    i8 iScale = pCol->iColumnScale();
    Fact* pScale = p_Scale_->pNewInstance();
    pScale->rxProperty().m_iInt = iScale;
    pColumn->vAttributeAdd(pScale);
    
    bool bIsNullable = pCol->bIsNullable();
    if (bIsNullable) {
      Fact* pNullable = p_Nullable_->pNewInstance();
      pColumn->vAttributeAdd(pNullable);
    }
    else {
      Fact* pNotNullable = p_NotNullable_->pNewInstance();
      pColumn->vAttributeAdd(pNotNullable);
    }
    
    bool bIsPtnKey = pCol->bIsPtnKey();
    if (bIsPtnKey) {
      Fact* pPtnKey = p_PtnKey_->pNewInstance();
      pColumn->vAttributeAdd(pPtnKey);
    }

  }
}


/*----------------------------------------------------------------------------*/
/* test                                                                       */
/*----------------------------------------------------------------------------*/
void Kbase::vTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
 
  m_pFact->vPrint_r(0,0);

  m_p_Concept_->vPrint_r(0,0);

  m_pContents->vPrint_r(0,1);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add apriori facts                                                          */
/*----------------------------------------------------------------------------*/
void Kbase::vAprioriFacts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);

  m_pFact = pFactNew(nullptr,"fact"_Stz);
  
  m_p_Concept_ = pFactNew(m_pFact,"_concept_"_Stz);

  
  m_p_Chunk_ = pConceptAdd(m_p_Concept_,"_chunk_"_Stz);
  m_p_Pattern_ = pConceptAdd(m_p_Concept_,"_pattern_"_Stz);
  m_p_Transform_ = pConceptAdd(m_p_Concept_,"_transform_"_Stz);

  
  m_p_Property_ = pConceptAdd(m_p_Concept_,"_property_"_Stz);
  m_p_Property_->rxProperty().m_eUsage = FU_USAGE;

  m_p_Function_ = pConceptAdd(m_p_Property_,"_function_"_Stz);
  m_p_Function_->rxProperty().m_eUsage = FU_FUNCTION;

  m_p_Int_ = pConceptAdd(m_p_Property_,"_int_"_Stz);
  m_p_Int_->rxProperty().m_eUsage = FU_INT;

  m_p_Object_ = pConceptAdd(m_p_Property_,"_object_"_Stz);
  m_p_Object_->rxProperty().m_eUsage = FU_OBJECT;

  m_p_Opaque_ = pConceptAdd(m_p_Property_,"_opaque_"_Stz);
  m_p_Opaque_->rxProperty().m_eUsage = FU_OPAQUE;

  m_p_Text_ = pConceptAdd(m_p_Property_,"_text_"_Stz);
  m_p_Text_->rxProperty().m_eUsage = FU_TEXT;

  m_p_Word_ = pConceptAdd(m_p_Property_,"_word_"_Stz);
  m_p_Word_->rxProperty().m_eUsage = FU_WORD;

  m_p_Xref_ = pConceptAdd(m_p_Property_,"_xref_"_Stz);
  m_p_Xref_->rxProperty().m_eUsage = FU_XREF;

  
  m_p_Name_ = pConceptAdd(m_p_Text_,"_name_"_Stz);
  
  m_p_Quality_ = pConceptAdd(m_p_Concept_,"_quality_"_Stz);
  
  m_p_Type_ = pConceptAdd(m_p_Concept_,"_type_"_Stz);
  m_p_Tuple_ = pConceptAdd(m_p_Concept_,"_tuple_"_Stz);
  m_p_Folder_ = pConceptAdd(m_p_Concept_,"_folder_"_Stz);

  Fact* p_Nullability_ = pConceptAdd(m_p_Quality_,"_nullability_"_Stz);
  Fact* p_ColumnType_  = pConceptAdd(m_p_Type_,"_columnType_"_Stz);
  Fact* p_Tables_      = pConceptAdd(m_p_Folder_,"_tables_"_Stz);
  Fact* p_Statement_   = pConceptAdd(m_p_Folder_,"_statement_"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pConceptAdd(p_ColumnType_,"BOOLEAN"_Stz);
  pConceptAdd(p_ColumnType_,"DATE"_Stz);
  pConceptAdd(p_ColumnType_,"DECIMAL"_Stz);
  pConceptAdd(p_ColumnType_,"FLOAT"_Stz);
  pConceptAdd(p_ColumnType_,"INTEGER"_Stz);
  pConceptAdd(p_ColumnType_,"INTERVAL"_Stz);
  pConceptAdd(p_ColumnType_,"TIME"_Stz);
  pConceptAdd(p_ColumnType_,"TIMESTAMP"_Stz);
  pConceptAdd(p_ColumnType_,"VARCHAR"_Stz);

  pConceptAdd(p_Nullability_,"_nullable_"_Stz);
  pConceptAdd(p_Nullability_,"_notNullable_"_Stz);

  pConceptAdd(m_p_Quality_,"_ptnKey_"_Stz);

  pConceptAdd(m_p_Folder_,"_table_"_Stz);
  pConceptAdd(m_p_Name_,"_tableNm_"_Stz);
  
  pConceptAdd(m_p_Folder_,"_columns_"_Stz);
  pConceptAdd(m_p_Tuple_,"_column_"_Stz);
  pConceptAdd(m_p_Name_,"_columnNm_"_Stz);
  pConceptAdd(m_p_Int_,"_length_"_Stz);
  pConceptAdd(m_p_Int_,"_scale_"_Stz);

  pConceptAdd(m_p_Folder_,"_indexes_"_Stz);
  pConceptAdd(m_p_Folder_,"_index_"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pContents  = pConceptAdd(m_p_Type_,"_contents_"_Stz);
  m_pContents   = pContents->pNewInstance();//pFactNew(m_pFolder,"contents"_Stz);

  Fact* pTables = p_Tables_->pNewInstance();
  m_pContents->vElementAdd(pTables);

  Fact* pStatement = p_Statement_->pNewInstance();
  m_pContents->vElementAdd(pStatement);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
