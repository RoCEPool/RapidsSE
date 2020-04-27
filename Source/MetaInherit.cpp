/*============================================================================*/
/* MetaInherit.cpp - Inheritance of another Meta Type                         */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaInherit,CN_METAINHERIT)
__aNewFunction(MetaInherit,CN_METAINHERIT)
__vDeleteFunction(MetaInherit,CN_METAINHERIT)
__aFooterFunction(MetaInherit)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaInherit::MetaInherit(Stz* szInheritedType, MetaInherits* p_WithinInherits)
: Object(CN_METAINHERIT)
{
  _precondition_(_MAGIC_,CN_METAINHERIT,_FOOTER_);
  
  m_p_WithinInherits = p_WithinInherits;
  
  m_sInheritedType   = szInheritedType;
  m_pTplArgs         = nullptr;
  
  _checkInitialised_(CN_METAINHERIT);
  
  _postcondition_(_MAGIC_,CN_METAINHERIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaInherit::~MetaInherit(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METAINHERIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the meta type that is inherited                                        */
/*----------------------------------------------------------------------------*/
Stz* MetaInherit::sInheritedType(void)
{
  _precondition_(_MAGIC_,CN_METAINHERIT,_FOOTER_);
  
  return m_sInheritedType;
}

/*----------------------------------------------------------------------------*/
/* get the optional template arguments                                        */
/*----------------------------------------------------------------------------*/
TagSet* MetaInherit::pTplArgs(void)
{
  _precondition_(_MAGIC_,CN_METAINHERIT,_FOOTER_);
  
  return m_pTplArgs;
}

/*----------------------------------------------------------------------------*/
/* set template arguments                                                     */
/*----------------------------------------------------------------------------*/
void MetaInherit::vTplArgs(TagSet* pTplArgs)
{
  _precondition_(_MAGIC_,CN_METAINHERIT,_FOOTER_);
  
  if (!m_pTplArgs)
    m_pTplArgs = new TagSet;
  
  m_pTplArgs->vCopy(pTplArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
