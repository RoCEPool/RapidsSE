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
Fact* Kbase::pFactNew(Stz* szNote, Fact* pSuper)
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
/* find element                                                                   */
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
  
  Fact* pTables  = m_pContents->pFindElement("tables"_Stz);
  Fact* pTable   = m_pConcepts->pFindElement("table"_Stz);
  Fact* pTableNm = m_pConcepts->pFindElement("tableNm"_Stz);
  Fact* pColumns = m_pConcepts->pFindElement("columns"_Stz);
  Fact* pColumn  = m_pConcepts->pFindElement("column"_Stz);
  Fact* pColumnNm = m_pConcepts->pFindElement("columnNm"_Stz);
  Fact* pColumnType = m_pConcepts->pFindElement("columnType"_Stz);
  Fact* pLength  = m_pConcepts->pFindElement("length"_Stz);
  Fact* pScale   = m_pConcepts->pFindElement("scale"_Stz);
  Fact* pNullable    = m_pConcepts->pFindElement("nullable"_Stz);
  Fact* pNotNullable = m_pConcepts->pFindElement("notNullable"_Stz);
  Fact* pPtnKey  = m_pConcepts->pFindElement("ptnKey"_Stz);

  Fact* pIsATable = pTable->pNewInstance(pTable->szNote());
  pTables->vElementAdd(pIsATable);

  Fact* pIsATableNm = pTableNm->pNewInstance(pTableToImport->szTableNm());
  pIsATableNm->rxProperty().m_pText = new Text;
  pIsATableNm->rxProperty().m_pText->vSet(pTableToImport->szTableNm());
  pIsATable->vAttributeAdd(pIsATableNm);

  Fact* pIsAColumns = pColumns->pNewInstance(pColumns->szNote());
  pIsATable->vElementAdd(pIsAColumns);

  StreamDef* pStmDef = pTableToImport->pStreamDef();
  
  i8 nCols = pStmDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pCol = pStmDef->pGetColumn(i_Col);
    
    Fact* pIsAColumn = pColumn->pNewInstance(pCol->szColNm());
    pIsAColumns->vElementAdd(pIsAColumn);
    
    Fact* pIsAColumnNm = pColumnNm->pNewInstance(pCol->szColNm());
    pIsAColumnNm->rxProperty().m_pText = new Text;
    pIsAColumnNm->rxProperty().m_pText->vSet(pCol->szColNm());
    pIsAColumn->vAttributeAdd(pIsAColumnNm);

    eCT eColType = pCol->eColumnType();
    Fact* pColType = m_pConcepts->pFindElement(aCT[eColType],pColumnType);
    Fact* pIsAColType = pColType->pNewInstance(aCT[eColType]);
    pIsAColumn->vAttributeAdd(pIsAColType);
    
    i8 iLength = pCol->iColumnLength();
    Fact* pIsALength = pLength->pNewInstance(pLength->szNote());
    pIsALength->rxProperty().m_iInt = iLength;
    pIsAColumn->vAttributeAdd(pIsALength);

    i8 iScale = pCol->iColumnScale();
    Fact* pIsAScale = pLength->pNewInstance(pScale->szNote());
    pIsAScale->rxProperty().m_iInt = iScale;
    pIsAColumn->vAttributeAdd(pIsAScale);
    
    bool bIsNullable = pCol->bIsNullable();
    if (bIsNullable) {
      Fact* pIsANullable = pNullable->pNewInstance(pNullable->szNote());
      pIsAColumn->vAttributeAdd(pIsANullable);
    }
    else {
      Fact* pIsANotNullable = pNotNullable->pNewInstance(pNotNullable->szNote());
      pIsAColumn->vAttributeAdd(pIsANotNullable);
    }
    
    bool bIsPtnKey = pCol->bIsPtnKey();
    if (bIsPtnKey) {
      Fact* pIsAPtnKey = pPtnKey->pNewInstance(pPtnKey->szNote());
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
    
  m_pContents->vPrint_r(0,0);
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

  m_pFact        = pFactNew("fact"_Stz,nullptr);
  m_pConcepts    = m_pFact->pNewInstance("glossary"_Stz);

  m_pProperty    = m_pFact->pNewInstance("property"_Stz);

  m_pAssociation = m_pProperty->pNewInstance("association"_Stz);
  m_pInt         = m_pProperty->pNewInstance("int"_Stz);
  m_pObject      = m_pProperty->pNewInstance("object"_Stz);
  m_pOpaque      = m_pProperty->pNewInstance("opaque"_Stz);
  m_pText        = m_pProperty->pNewInstance("text"_Stz);
  m_pWord        = m_pProperty->pNewInstance("word"_Stz);

  m_pName        = m_pText->pNewInstance("name"_Stz);
  
  m_pQuality     = m_pFact->pNewInstance("quality"_Stz);
  
  m_pType        = m_pFact->pNewInstance("type"_Stz);
  m_pTuple       = m_pFact->pNewInstance("tuple"_Stz);
  m_pFolder      = m_pFact->pNewInstance("folder"_Stz);
  
  m_pContents    = m_pFolder->pNewInstance("contents"_Stz);

  Fact* pTables      = m_pFolder->pNewInstance("tables"_Stz);

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

  Fact* pTable       = pConceptAdd(m_pFolder,"table"_Stz);
  Fact* pTableDef    = pConceptAdd(m_pTuple,"tableDef"_Stz);
  Fact* pTableNm     = pConceptAdd(m_pName,"tableNm"_Stz);
  
  Fact* pColumns     = pConceptAdd(m_pFolder,"columns"_Stz);
  Fact* pColumn      = pConceptAdd(m_pTuple,"column"_Stz);
  Fact* pColumnNm    = pConceptAdd(m_pName,"columnNm"_Stz);
  Fact* pLength      = pConceptAdd(m_pInt,"length"_Stz);
  Fact* pScale       = pConceptAdd(m_pInt,"scale"_Stz);

  Fact* pIndexes     = pConceptAdd(m_pFolder,"indexes"_Stz);
  Fact* pIndex       = pConceptAdd(m_pFolder,"index"_Stz);
  Fact* pIndexDef    = pConceptAdd(m_pTuple,"indexDef"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //m_pConcepts->vElementAdd(pColumnType);
//  m_pConcepts->vElementAdd(pBOOLEAN);
//  m_pConcepts->vElementAdd(pDATE);
//  m_pConcepts->vElementAdd(pDECIMAL);
//  m_pConcepts->vElementAdd(pFLOAT);
//  m_pConcepts->vElementAdd(pINTEGER);
//  m_pConcepts->vElementAdd(pINTERVAL);
//  m_pConcepts->vElementAdd(pTIME);
//  m_pConcepts->vElementAdd(pTIMESTAMP);
//  m_pConcepts->vElementAdd(pVARCHAR);
//
//  m_pConcepts->vElementAdd(pNullability);
//  m_pConcepts->vElementAdd(pNullable);
//  m_pConcepts->vElementAdd(pNotNullable);
//
//  m_pConcepts->vElementAdd(pPtnKey);
//
//  //m_pConcepts->vElementAdd(pTables);
//  m_pConcepts->vElementAdd(pTable);
//  m_pConcepts->vElementAdd(pTableDef);
//  m_pConcepts->vElementAdd(pTableNm);
//
//  m_pConcepts->vElementAdd(pColumns);
//  m_pConcepts->vElementAdd(pColumn);
//  m_pConcepts->vElementAdd(pColumnNm);
//  m_pConcepts->vElementAdd(pLength);
//  m_pConcepts->vElementAdd(pScale);
//
//  m_pConcepts->vElementAdd(pIndexes);
//  m_pConcepts->vElementAdd(pIndex);
//  m_pConcepts->vElementAdd(pIndexDef);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pContents->vElementAdd(pTables);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
