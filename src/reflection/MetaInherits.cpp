/*============================================================================*/
/* MetaInherits.cpp - Meta Inheritance List                                   */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaInherits,CN_METAINHERITS)
__aNewFunction(MetaInherits,CN_METAINHERITS)
__vDeleteFunction(MetaInherits,CN_METAINHERITS)
__aFooterFunction(MetaInherits)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaInherits::MetaInherits(MetaType* p_MetaType)
: Object(CN_METAINHERITS)
{
  _precondition_(_MAGIC_,CN_METAINHERITS,_FOOTER_);
  
  m_p_MetaType = p_MetaType;

  _checkInitialised_(CN_METAINHERITS);
  
  _postcondition_(_MAGIC_,CN_METAINHERITS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaInherits::~MetaInherits(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METAINHERITS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a new member with given name                                           */
/*----------------------------------------------------------------------------*/
MetaInherit* MetaInherits::pAddInherit(Stz* szBaseClassNm)
{
  _precondition_(_MAGIC_,CN_METAINHERITS,_FOOTER_);
  
  MetaInherit* pMetaInherit = new MetaInherit(szBaseClassNm,this);

  vAppend(pMetaInherit);
  
  return pMetaInherit;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
