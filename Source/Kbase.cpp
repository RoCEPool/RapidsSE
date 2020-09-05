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
void Kbase::vImport(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  vTrace("Import %s ...\n"_Stz,pTable->szTableNm());
}


/*----------------------------------------------------------------------------*/
/* test                                                                       */
/*----------------------------------------------------------------------------*/
void Kbase::vTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  m_pContents->vPrint_r(0);
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

  m_pProperty    = m_pFact->pNewInstance("property"_Stz);

  m_pAssociation = m_pProperty->pNewInstance("association"_Stz);
  m_pInt         = m_pProperty->pNewInstance("int"_Stz);
  m_pObject      = m_pProperty->pNewInstance("object"_Stz);
  m_pOpaque      = m_pProperty->pNewInstance("opaque"_Stz);
  m_pText        = m_pProperty->pNewInstance("text"_Stz);
  m_pWord        = m_pProperty->pNewInstance("word"_Stz);

  m_pName        = m_pText->pNewInstance("name"_Stz);
  
  m_pQuality     = m_pAssociation->pNewInstance("quality"_Stz);
  
  m_pType        = m_pFact->pNewInstance("type"_Stz);
  m_pTuple       = m_pFact->pNewInstance("tuple"_Stz);
  m_pFolder      = m_pFact->pNewInstance("folder"_Stz);
  
  m_pContents    = m_pFolder->pNewInstance("contents"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pColumnType  = m_pType->pNewInstance("columnType"_Stz);
  Fact* pBOOLEAN     = pColumnType->pNewInstance("BOOLEAN"_Stz);
  Fact* pDATE        = pColumnType->pNewInstance("DATE"_Stz);
  Fact* pDECIMAL     = pColumnType->pNewInstance("DECIMAL"_Stz);
  Fact* pFLOAT       = pColumnType->pNewInstance("FLOAT"_Stz);
  Fact* pINTEGER     = pColumnType->pNewInstance("INTEGER"_Stz);
  Fact* pINTERVAL    = pColumnType->pNewInstance("INTERVAL"_Stz);
  Fact* pTIME        = pColumnType->pNewInstance("TIME"_Stz);
  Fact* pTIMESTAMP   = pColumnType->pNewInstance("TIMESTAMP"_Stz);
  Fact* pVARCHAR     = pColumnType->pNewInstance("VARCHAR"_Stz);

  Fact* pNullability = m_pQuality->pNewInstance("nullability"_Stz);
  Fact* pNullable    = pNullability->pNewInstance("nullable"_Stz);
  Fact* pNotNullable = pNullability->pNewInstance("notNullable"_Stz);

  Fact* pTables      = m_pFolder->pNewInstance("tables"_Stz);
  Fact* pTable       = m_pFolder->pNewInstance("table"_Stz);
  Fact* pTableDef    = m_pTuple->pNewInstance("tableDef"_Stz);
  Fact* pTableNm     = m_pName->pNewInstance("tableNm"_Stz);
  
  Fact* pColumns     = m_pFolder->pNewInstance("columns"_Stz);
  Fact* pColumn      = m_pTuple->pNewInstance("column"_Stz);
  Fact* pColumnNm    = m_pName->pNewInstance("columnNm"_Stz);
  Fact* pWidth       = m_pInt->pNewInstance("width"_Stz);
  Fact* pScale       = m_pInt->pNewInstance("scale"_Stz);

  Fact* pIndexes     = m_pFolder->pNewInstance("indexes"_Stz);
  Fact* pIndex       = m_pFolder->pNewInstance("index"_Stz);
  Fact* pIndexDef    = m_pTuple->pNewInstance("indexDef"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pContents->vElementAdd(pTables);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
