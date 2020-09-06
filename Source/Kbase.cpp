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
  
  Fact* pElement = m_pIsAContents->pFindElement(szNote,pSuper);

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
/* import table                                                               */
/*----------------------------------------------------------------------------*/
void Kbase::vImport(Table* pTableToImport)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  vTrace("Import %s ...\n"_Stz,pTableToImport->szTableNm());
  
  Fact* pIsATables   = m_pIsAContents->pFindElement("tables"_Stz);
  Fact* pTable       = m_p_Concept_->pFindElement("_table_"_Stz);
  Fact* pTableNm     = m_p_Concept_->pFindElement("_tableNm_"_Stz);
  Fact* pColumns     = m_p_Concept_->pFindElement("_columns_"_Stz);
  Fact* pColumn      = m_p_Concept_->pFindElement("_column_"_Stz);
  Fact* pColumnNm    = m_p_Concept_->pFindElement("_columnNm_"_Stz);
  Fact* pColumnType  = m_p_Concept_->pFindElement("_columnType_"_Stz);
  Fact* pLength      = m_p_Concept_->pFindElement("_length_"_Stz);
  Fact* pScale       = m_p_Concept_->pFindElement("_scale_"_Stz);
  Fact* pNullable    = m_p_Concept_->pFindElement("_nullable_"_Stz);
  Fact* pNotNullable = m_p_Concept_->pFindElement("_notNullable_"_Stz);
  Fact* pPtnKey      = m_p_Concept_->pFindElement("_ptnKey_"_Stz);

  Fact* pIsATable = pTable->pNewInstance();
  pIsATables->vElementAdd(pIsATable);

  Fact* pIsATableNm = pTableNm->pNewInstance();
  pIsATableNm->rxProperty().m_pText = new Text;
  pIsATableNm->rxProperty().m_pText->vSet(pTableToImport->szTableNm());
  pIsATable->vAttributeAdd(pIsATableNm);

  Fact* pIsAColumns = pColumns->pNewInstance();
  pIsATable->vElementAdd(pIsAColumns);

  StreamDef* pStmDef = pTableToImport->pStreamDef();
  
  i8 nCols = pStmDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pCol = pStmDef->pGetColumn(i_Col);
    
    Fact* pIsAColumn = pColumn->pNewInstance();
    pIsAColumns->vElementAdd(pIsAColumn);
    
    Fact* pIsAColumnNm = pColumnNm->pNewInstance();
    pIsAColumnNm->rxProperty().m_pText = new Text;
    pIsAColumnNm->rxProperty().m_pText->vSet(pCol->szColNm());
    pIsAColumn->vAttributeAdd(pIsAColumnNm);

    eCT eColType = pCol->eColumnType();
    Fact* pColType = m_p_Concept_->pFindElement(aCT[eColType],pColumnType);
    Fact* pIsAColType = pColType->pNewInstance();
    pIsAColumn->vAttributeAdd(pIsAColType);
    
    i8 iLength = pCol->iColumnLength();
    Fact* pIsALength = pLength->pNewInstance();
    pIsALength->rxProperty().m_iInt = iLength;
    pIsAColumn->vAttributeAdd(pIsALength);

    i8 iScale = pCol->iColumnScale();
    Fact* pIsAScale = pScale->pNewInstance();
    pIsAScale->rxProperty().m_iInt = iScale;
    pIsAColumn->vAttributeAdd(pIsAScale);
    
    bool bIsNullable = pCol->bIsNullable();
    if (bIsNullable) {
      Fact* pIsANullable = pNullable->pNewInstance();
      pIsAColumn->vAttributeAdd(pIsANullable);
    }
    else {
      Fact* pIsANotNullable = pNotNullable->pNewInstance();
      pIsAColumn->vAttributeAdd(pIsANotNullable);
    }
    
    bool bIsPtnKey = pCol->bIsPtnKey();
    if (bIsPtnKey) {
      Fact* pIsAPtnKey = pPtnKey->pNewInstance();
      pIsAColumn->vAttributeAdd(pIsAPtnKey);
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

  m_pIsAContents->vPrint_r(0,1);
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

  m_pFact        = pFactNew(nullptr,"fact"_Stz);
  
  m_p_Concept_     = pFactNew(m_pFact,"_concept_"_Stz);

  m_p_Property_    = pConceptAdd(m_p_Concept_,"_property_"_Stz);
  m_p_Property_->rxProperty().m_eUsage = FU_USAGE;

  m_p_Int_         = pConceptAdd(m_p_Property_,"_int_"_Stz);
  m_p_Int_->rxProperty().m_eUsage = FU_INT;

  m_p_Object_      = pConceptAdd(m_p_Property_,"_object_"_Stz);
  m_p_Object_->rxProperty().m_eUsage = FU_OBJECT;

  m_p_Opaque_      = pConceptAdd(m_p_Property_,"_opaque_"_Stz);
  m_p_Opaque_->rxProperty().m_eUsage = FU_OPAQUE;

  m_p_Text_        = pConceptAdd(m_p_Property_,"_text_"_Stz);
  m_p_Text_->rxProperty().m_eUsage = FU_TEXT;

  m_p_Word_        = pConceptAdd(m_p_Property_,"_word_"_Stz);
  m_p_Word_->rxProperty().m_eUsage = FU_WORD;

  m_p_Xref_        = pConceptAdd(m_p_Property_,"_xref_"_Stz);
  m_p_Xref_->rxProperty().m_eUsage = FU_XREF;

  
  m_p_Name_        = pConceptAdd(m_p_Text_,"_name_"_Stz);
  
  m_p_Quality_    = pConceptAdd(m_p_Concept_,"_quality_"_Stz);
  
  m_p_Type_        = pConceptAdd(m_p_Concept_,"_type_"_Stz);
  m_p_Tuple_       = pConceptAdd(m_p_Concept_,"_tuple_"_Stz);
  m_p_Folder_      = pConceptAdd(m_p_Concept_,"_folder_"_Stz);

  Fact* pNullability = pConceptAdd(m_p_Quality_,"_nullability_"_Stz);
  Fact* pColumnType  = pConceptAdd(m_p_Type_,"_columnType_"_Stz);
  Fact* pTables      = pConceptAdd(m_p_Folder_,"_tables_"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pConceptAdd(pColumnType,"BOOLEAN"_Stz);
  pConceptAdd(pColumnType,"DATE"_Stz);
  pConceptAdd(pColumnType,"DECIMAL"_Stz);
  pConceptAdd(pColumnType,"FLOAT"_Stz);
  pConceptAdd(pColumnType,"INTEGER"_Stz);
  pConceptAdd(pColumnType,"INTERVAL"_Stz);
  pConceptAdd(pColumnType,"TIME"_Stz);
  pConceptAdd(pColumnType,"TIMESTAMP"_Stz);
  pConceptAdd(pColumnType,"VARCHAR"_Stz);

  pConceptAdd(pNullability,"_nullable_"_Stz);
  pConceptAdd(pNullability,"_notNullable_"_Stz);

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
  m_pIsAContents   = pContents->pNewInstance();//pFactNew(m_pFolder,"contents"_Stz);

  Fact* pIsATables = pTables->pNewInstance();
  m_pIsAContents->vElementAdd(pIsATables);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
