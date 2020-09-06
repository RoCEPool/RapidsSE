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
  
  m_pConcepts->vElementAdd(pConcept);

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
  
  Fact* pIsATables   = m_pContents->pFindElement("+ tables"_Stz);
  Fact* pTable       = m_pConcepts->pFindElement("table"_Stz);
  Fact* pTableNm     = m_pConcepts->pFindElement("tableNm"_Stz);
  Fact* pColumns     = m_pConcepts->pFindElement("columns"_Stz);
  Fact* pColumn      = m_pConcepts->pFindElement("column"_Stz);
  Fact* pColumnNm    = m_pConcepts->pFindElement("columnNm"_Stz);
  Fact* pColumnType  = m_pConcepts->pFindElement("columnType"_Stz);
  Fact* pLength      = m_pConcepts->pFindElement("length"_Stz);
  Fact* pScale       = m_pConcepts->pFindElement("scale"_Stz);
  Fact* pNullable    = m_pConcepts->pFindElement("nullable"_Stz);
  Fact* pNotNullable = m_pConcepts->pFindElement("notNullable"_Stz);
  Fact* pPtnKey      = m_pConcepts->pFindElement("ptnKey"_Stz);

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
    Fact* pColType = m_pConcepts->pFindElement(aCT[eColType],pColumnType);
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

  m_pConcepts->vPrint_r(0,0);

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

  m_pFact        = pFactNew(nullptr,"fact"_Stz);
  m_pConcepts    = pFactNew(m_pFact,"concepts"_Stz);

  m_pProperty    = pConceptAdd(m_pFact,"property"_Stz);
  m_pProperty->rxProperty().m_eUsage = FU_USAGE;

  m_pInt         = pConceptAdd(m_pProperty,"int"_Stz);
  m_pInt->rxProperty().m_eUsage = FU_INT;

  m_pObject      = pConceptAdd(m_pProperty,"object"_Stz);
  m_pObject->rxProperty().m_eUsage = FU_OBJECT;

  m_pOpaque      = pConceptAdd(m_pProperty,"opaque"_Stz);
  m_pOpaque->rxProperty().m_eUsage = FU_OPAQUE;

  m_pText        = pConceptAdd(m_pProperty,"text"_Stz);
  m_pText->rxProperty().m_eUsage = FU_TEXT;

  m_pWord        = pConceptAdd(m_pProperty,"word"_Stz);
  m_pWord->rxProperty().m_eUsage = FU_WORD;

  m_pXref        = pConceptAdd(m_pProperty,"xref"_Stz);
  m_pXref->rxProperty().m_eUsage = FU_XREF;

  
  m_pName        = pConceptAdd(m_pText,"name"_Stz);
  
  m_pQuality     = pConceptAdd(m_pFact,"quality"_Stz);
  
  m_pType        = pConceptAdd(m_pFact,"type"_Stz);
  m_pTuple       = pConceptAdd(m_pFact,"tuple"_Stz);
  m_pFolder      = pConceptAdd(m_pFact,"folder"_Stz);
  
  m_pContents    = pFactNew(m_pFolder,"contents"_Stz);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pColumnType  = pConceptAdd(m_pType,"columnType"_Stz);
  Fact* pBOOLEAN     = pConceptAdd(pColumnType,"BOOLEAN"_Stz);
  Fact* pDATE        = pConceptAdd(pColumnType,"DATE"_Stz);
  Fact* pDECIMAL     = pConceptAdd(pColumnType,"DECIMAL"_Stz);
  Fact* pFLOAT       = pConceptAdd(pColumnType,"FLOAT"_Stz);
  Fact* pINTEGER     = pConceptAdd(pColumnType,"INTEGER"_Stz);
  Fact* pINTERVAL    = pConceptAdd(pColumnType,"INTERVAL"_Stz);
  Fact* pTIME        = pConceptAdd(pColumnType,"TIME"_Stz);
  Fact* pTIMESTAMP   = pConceptAdd(pColumnType,"TIMESTAMP"_Stz);
  Fact* pVARCHAR     = pConceptAdd(pColumnType,"VARCHAR"_Stz);

  Fact* pNullability = pConceptAdd(m_pQuality,"nullability"_Stz);
  Fact* pNullable    = pConceptAdd(pNullability,"nullable"_Stz);
  Fact* pNotNullable = pConceptAdd(pNullability,"notNullable"_Stz);

  Fact* pPtnKey      = pConceptAdd(m_pQuality,"ptnKey"_Stz);

  Fact* pTables      = pConceptAdd(m_pFolder,"tables"_Stz);
  Fact* pTable       = pConceptAdd(m_pFolder,"table"_Stz);
//  Fact* pTableDef    = pConceptAdd(m_pTuple,"tableDef"_Stz);
  Fact* pTableNm     = pConceptAdd(m_pName,"tableNm"_Stz);
  
  Fact* pColumns     = pConceptAdd(m_pFolder,"columns"_Stz);
  Fact* pColumn      = pConceptAdd(m_pTuple,"column"_Stz);
  Fact* pColumnNm    = pConceptAdd(m_pName,"columnNm"_Stz);
  Fact* pLength      = pConceptAdd(m_pInt,"length"_Stz);
  Fact* pScale       = pConceptAdd(m_pInt,"scale"_Stz);

  Fact* pIndexes     = pConceptAdd(m_pFolder,"indexes"_Stz);
  Fact* pIndex       = pConceptAdd(m_pFolder,"index"_Stz);
//  Fact* pIndexDef    = pConceptAdd(m_pTuple,"indexDef"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pIsATables = pTables->pNewInstance();
  m_pContents->vElementAdd(pIsATables);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
