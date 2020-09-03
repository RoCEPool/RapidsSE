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
Fact::Fact(i8 iFactNo, Fact* pParent, Stz* szNote)
: Object(CN_FACT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_iFactNo = iFactNo;
  m_pParent = pParent;
  m_sNote   = szNote ? szNote : C_NO_NOTE;
  
  m_xProperty.m_pObject = nullptr;
  
  m_pAttributes = new FactVec;



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
  
  delete m_pAttributes;

  delete m_pFacetVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add attribute (sub-fact)                                                   */
/*----------------------------------------------------------------------------*/
void Fact::vAddAttribute(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  m_pAttributes->vAdd(pAttribute);
}
 
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
Stz* Fact::szNote(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_sNote;
}

/*----------------------------------------------------------------------------*/
/* set fact note                                                              */
/*----------------------------------------------------------------------------*/
void Fact::vNote(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_sNote = szNote;
}

/*----------------------------------------------------------------------------*/
/* get parent                                                                 */
/*----------------------------------------------------------------------------*/
Fact* Fact::pParent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pParent;
}

/*----------------------------------------------------------------------------*/
/* set parent                                                                 */
/*----------------------------------------------------------------------------*/
void Fact::vParent(Fact* pParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pParent = pParent;
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
