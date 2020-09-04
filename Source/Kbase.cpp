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
  
  i8 iFactNo = iNextFactNo();

  Fact* pFact = new Fact(iFactNo, pSuper, szNote);

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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add apriori facts                                                          */
/*----------------------------------------------------------------------------*/
void Kbase::vAprioriFacts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);

  m_pFact        = pFactNew("fact"_Stz,nullptr);

  m_pAssociation = pFactNew("association"_Stz,m_pFact);
  m_pInt         = pFactNew("int"_Stz,m_pFact);
  m_pObject      = pFactNew("object"_Stz,m_pFact);
  m_pOpaque      = pFactNew("opaque"_Stz,m_pFact);
  m_pText        = pFactNew("text"_Stz,m_pFact);
  m_pWord        = pFactNew("word"_Stz,m_pFact);

  m_pName        = pFactNew("name"_Stz,m_pText);
  
  m_pQuality     = pFactNew("type"_Stz,m_pFact);
  m_pType        = pFactNew("type"_Stz,m_pFact);
  m_pTuple       = pFactNew("tuple"_Stz,m_pFact);
  m_pFolder      = pFactNew("folder"_Stz,m_pFact);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pColumnType  = pFactNew("columnType"_Stz,m_pType);
  Fact* pBOOLEAN     = pFactNew("BOOLEAN"_Stz,pColumnType);
  Fact* pDATE        = pFactNew("DATE"_Stz,pColumnType);
  Fact* pDECIMAL     = pFactNew("DECIMAL"_Stz,pColumnType);
  Fact* pFLOAT       = pFactNew("FLOAT"_Stz,pColumnType);
  Fact* pINTEGER     = pFactNew("INTEGER"_Stz,pColumnType);
  Fact* pINTERVAL    = pFactNew("INTERVAL"_Stz,pColumnType);
  Fact* pTIME        = pFactNew("TIME"_Stz,pColumnType);
  Fact* pTIMESTAMP   = pFactNew("TIMESTAMP"_Stz,pColumnType);
  Fact* pVARCHAR     = pFactNew("VARCHAR"_Stz,pColumnType);

  Fact* pNullability = pFactNew("nullability"_Stz,m_pQuality);
  Fact* pNullable    = pFactNew("nullable"_Stz,pNullability);
  Fact* pNotNullable = pFactNew("notNullable"_Stz,pNullability);

  Fact* pTables      = pFactNew("tables"_Stz,m_pFolder);
  Fact* pTable       = pFactNew("table"_Stz,m_pFolder);
  Fact* pTableDef    = pFactNew("tableDef"_Stz,m_pTuple);
  Fact* pTableNm     = pFactNew("tableNm"_Stz,m_pName);
  
  Fact* pColumns     = pFactNew("columns"_Stz,m_pFolder);
  Fact* pColumn      = pFactNew("column"_Stz,m_pTuple);
  Fact* pColumnNm    = pFactNew("columnNm"_Stz,m_pName);
  Fact* pWidth       = pFactNew("width"_Stz,m_pInt);
  Fact* pScale       = pFactNew("scale"_Stz,m_pInt);

  Fact* pIndexes     = pFactNew("indexes"_Stz,m_pFolder);
  Fact* pIndex       = pFactNew("index"_Stz,m_pFolder);
  Fact* pIndexDef    = pFactNew("indexDef"_Stz,m_pTuple);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
