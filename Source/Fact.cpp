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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add attribute (a sub-fact)                                                 */
/*----------------------------------------------------------------------------*/
void Fact::vAddAttribute(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  m_pAttributes->vAdd(pAttribute);
}
 
/*----------------------------------------------------------------------------*/
/* get attributes (the set of sub-facts)                                      */
/*----------------------------------------------------------------------------*/
FactVec* Fact::pAttributes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return m_pAttributes;
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
/* get attribute that is a sub-class of given fact                            */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeThatIsSubClass(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  i8 nAtr = m_pAttributes->nItems();
  
  for (i8 i_Atr = 0; i_Atr < nAtr; i_Atr++) {
    Fact* pAttribute = m_pAttributes->pGet(i_Atr);
    if (pAttribute->bIsSubClass(pOfFact))
      return pAttribute;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* has an attribute which specifically given fact                             */
/*----------------------------------------------------------------------------*/
bool Fact::bHasSpecificAttribute(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  i8 nAtr = m_pAttributes->nItems();
  
  for (i8 i_Atr = 0; i_Atr < nAtr; i_Atr++) {
    Fact* pAttribute = m_pAttributes->pGet(i_Atr);
    if (pAttribute == pFact)
      return true;
  }

  return false;
}
 
/*----------------------------------------------------------------------------*/
/* is this fact a sub-class of given fact                         (recursive) */
/*----------------------------------------------------------------------------*/
bool Fact::bIsSubClass(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (this == pOfFact)
    return true;
  
  if (! m_pParent)
    return false;
  
  return m_pParent->bIsSubClass(pOfFact);
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
/* get parent fact                                                            */
/*----------------------------------------------------------------------------*/
Fact* Fact::pParent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pParent;
}

/*----------------------------------------------------------------------------*/
/* set parent fact                                                            */
/*----------------------------------------------------------------------------*/
void Fact::vParent(Fact* pParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pParent = pParent;
}

/*----------------------------------------------------------------------------*/
/* get property                                                               */
/*----------------------------------------------------------------------------*/
Property& Fact::rxProperty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_xProperty;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
