/*============================================================================*/
/* Fact.cpp - Fact                                                            */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Fact,CN_FACT)
__aNewFunction(Fact,CN_FACT)
__vDeleteFunction(Fact,CN_FACT)
__aFooterFunction(Fact)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fact::Fact(i8 iFactNo, Fact* pPropertyKind, Stz* szFactNote)
: Object(CN_FACT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_iFactNo = iFactNo;
  m_sFactNote = szFactNote ? szFactNote : C_NO_NOTE;
  m_pPropertyKind = pPropertyKind;
  m_xProperty.m_pObject = nullptr;

  m_pFacetVec = new FacetVec;
  
  _checkInitialised_(CN_FACT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fact::~Fact(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  delete m_pFacetVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get fact no                                                                */
/*----------------------------------------------------------------------------*/
i8 Fact::iFactNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_iFactNo;
}

/*----------------------------------------------------------------------------*/
/* set fact no                                                                */
/*----------------------------------------------------------------------------*/
void Fact::vFactNo(i8 iFactNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_iFactNo = iFactNo;
}

/*----------------------------------------------------------------------------*/
/* get fact name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Fact::szFactNote(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_sFactNote;
}

/*----------------------------------------------------------------------------*/
/* set fact name                                                              */
/*----------------------------------------------------------------------------*/
void Fact::vFactNote(Stz* szFactNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_sFactNote = szFactNote;
}

/*----------------------------------------------------------------------------*/
/* get property kind                                                          */
/*----------------------------------------------------------------------------*/
Property& Fact::rxProperty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_xProperty;
}

/*----------------------------------------------------------------------------*/
/* get property kind                                                          */
/*----------------------------------------------------------------------------*/
Fact* Fact::pPropertyKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pPropertyKind;
}

/*----------------------------------------------------------------------------*/
/* set property kind                                                          */
/*----------------------------------------------------------------------------*/
void Fact::vPropertyKind(Fact* pPropertyKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pPropertyKind = pPropertyKind;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add facet                                                                  */
/*----------------------------------------------------------------------------*/
void Fact::vAddFacet(Facet* pFacet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  m_pFacetVec->vAdd(pFacet);
}
 
/*----------------------------------------------------------------------------*/
/* add facet                                                                  */
/*----------------------------------------------------------------------------*/
void Fact::vAddFacet(Fact* pAttributeKind, Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  Facet* pFacet = new Facet(pAttributeKind,pAttribute);
  
  m_pFacetVec->vAdd(pFacet);
}
 
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
