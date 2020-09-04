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
Fact* Kbase::pFactNew(Stz* szNote, Fact* pParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFact = new Fact(++ m_ixFactNo, pParent, szNote);

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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add apriori facts                                                          */
/*----------------------------------------------------------------------------*/
void Kbase::vAprioriFacts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);

  m_pFact        = pFactNew("fact"_Stz,nullptr);

  m_pContents    = pFactNew("contents"_Stz,m_pFact);

  m_pAssociation = pFactNew("association"_Stz,m_pFact);
  m_pInt         = pFactNew("int"_Stz,m_pFact);
  m_pObject      = pFactNew("object"_Stz,m_pFact);
  m_pOpaque      = pFactNew("opaque"_Stz,m_pFact);
  m_pText        = pFactNew("text"_Stz,m_pFact);
  m_pWord        = pFactNew("word"_Stz,m_pFact);

  m_pSet         = pFactNew("set"_Stz,m_pFact);
  m_pElement     = pFactNew("element"_Stz,m_pFact);
  m_pFirst       = pFactNew("first"_Stz,m_pFact);
  m_pLast        = pFactNew("last"_Stz,m_pFact);
  m_pNext        = pFactNew("next"_Stz,m_pFact);
  m_pPrev        = pFactNew("prev"_Stz,m_pFact);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pName        = pFactNew("name"_Stz,m_pText);

  Fact* pTables      = pFactNew("tables"_Stz,m_pSet);
  Fact* pTable       = pFactNew("table"_Stz,m_pElement);
  Fact* pTableNm     = pFactNew("tableNm"_Stz,pName);
  
  Fact* pColumns     = pFactNew("columns"_Stz,m_pSet);
  Fact* pColumn      = pFactNew("column"_Stz,m_pElement);
  Fact* pColumnNm    = pFactNew("columnNm"_Stz,pName);
  Fact* pColumnType  = pFactNew("columnType"_Stz,m_pFact);
  Fact* pWidth       = pFactNew("width"_Stz,m_pFact);
  Fact* pScale       = pFactNew("scale"_Stz,m_pFact);
  Fact* pNullability = pFactNew("nullability"_Stz,m_pFact);
  Fact* pNullable    = pFactNew("nullable"_Stz,pNullability);
  Fact* pNotNullable = pFactNew("notNullable"_Stz,pNullability);

  Fact* pBOOLEAN     = pFactNew("BOOLEAN"_Stz,pColumnType);
  Fact* pDATE        = pFactNew("DATE"_Stz,pColumnType);
  Fact* pDECIMAL     = pFactNew("DECIMAL"_Stz,pColumnType);
  Fact* pFLOAT       = pFactNew("FLOAT"_Stz,pColumnType);
  Fact* pINTEGER     = pFactNew("INTEGER"_Stz,pColumnType);
  Fact* pINTERVAL    = pFactNew("INTERVAL"_Stz,pColumnType);
  Fact* pTIME        = pFactNew("TIME"_Stz,pColumnType);
  Fact* pTIMESTAMP   = pFactNew("TIMESTAMP"_Stz,pColumnType);
  Fact* pVARCAHR     = pFactNew("VARCHAR"_Stz,pColumnType);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
