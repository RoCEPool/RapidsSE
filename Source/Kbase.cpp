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
  
  m_pConcept->vElementAdd(pConcept);

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
  
  return m_pProperty;
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
  
  Fact* pIsATables   = m_pIsAContents->pFindElement("+ _tables_"_Stz);
  Fact* pTable       = m_pConcept->pFindElement("_table_"_Stz);
  Fact* pTableNm     = m_pConcept->pFindElement("_tableNm_"_Stz);
  Fact* pColumns     = m_pConcept->pFindElement("_columns_"_Stz);
  Fact* pColumn      = m_pConcept->pFindElement("_column_"_Stz);
  Fact* pColumnNm    = m_pConcept->pFindElement("_columnNm_"_Stz);
  Fact* pColumnType  = m_pConcept->pFindElement("_columnType_"_Stz);
  Fact* pLength      = m_pConcept->pFindElement("_length_"_Stz);
  Fact* pScale       = m_pConcept->pFindElement("_scale_"_Stz);
  Fact* pNullable    = m_pConcept->pFindElement("_nullable_"_Stz);
  Fact* pNotNullable = m_pConcept->pFindElement("_notNullable_"_Stz);
  Fact* pPtnKey      = m_pConcept->pFindElement("_ptnKey_"_Stz);

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
    Fact* pColType = m_pConcept->pFindElement(aCT[eColType],pColumnType);
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

  m_pConcept->vPrint_r(0,0);

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
  m_pConcept     = pFactNew(m_pFact,"_concept_"_Stz);

  m_pProperty    = pConceptAdd(m_pConcept,"_property_"_Stz);
  m_pProperty->rxProperty().m_eUsage = FU_USAGE;

  m_pInt         = pConceptAdd(m_pProperty,"_int_"_Stz);
  m_pInt->rxProperty().m_eUsage = FU_INT;

  m_pObject      = pConceptAdd(m_pProperty,"_object_"_Stz);
  m_pObject->rxProperty().m_eUsage = FU_OBJECT;

  m_pOpaque      = pConceptAdd(m_pProperty,"_opaque_"_Stz);
  m_pOpaque->rxProperty().m_eUsage = FU_OPAQUE;

  m_pText        = pConceptAdd(m_pProperty,"_text_"_Stz);
  m_pText->rxProperty().m_eUsage = FU_TEXT;

  m_pWord        = pConceptAdd(m_pProperty,"_word_"_Stz);
  m_pWord->rxProperty().m_eUsage = FU_WORD;

  m_pXref        = pConceptAdd(m_pProperty,"_xref_"_Stz);
  m_pXref->rxProperty().m_eUsage = FU_XREF;

  
  m_pName        = pConceptAdd(m_pText,"_name_"_Stz);
  
  m_pQuality     = pConceptAdd(m_pConcept,"_quality_"_Stz);
  
  m_pType        = pConceptAdd(m_pConcept,"_type_"_Stz);
  m_pTuple       = pConceptAdd(m_pConcept,"_tuple_"_Stz);
  m_pFolder      = pConceptAdd(m_pConcept,"_folder_"_Stz);

  Fact* pNullability = pConceptAdd(m_pQuality,"_nullability_"_Stz);
  Fact* pColumnType  = pConceptAdd(m_pType,"_columnType_"_Stz);
  Fact* pTables      = pConceptAdd(m_pFolder,"_tables_"_Stz);

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

  pConceptAdd(m_pQuality,"_ptnKey_"_Stz);

  pConceptAdd(m_pFolder,"_table_"_Stz);
  pConceptAdd(m_pName,"_tableNm_"_Stz);
  
  pConceptAdd(m_pFolder,"_columns_"_Stz);
  pConceptAdd(m_pTuple,"_column_"_Stz);
  pConceptAdd(m_pName,"_columnNm_"_Stz);
  pConceptAdd(m_pInt,"_length_"_Stz);
  pConceptAdd(m_pInt,"_scale_"_Stz);

  pConceptAdd(m_pFolder,"_indexes_"_Stz);
  pConceptAdd(m_pFolder,"_index_"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pContents  = pConceptAdd(m_pType,"_contents_"_Stz);
  m_pIsAContents   = pContents->pNewInstance();//pFactNew(m_pFolder,"contents"_Stz);

  Fact* pIsATables = pTables->pNewInstance();
  m_pIsAContents->vElementAdd(pIsATables);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
