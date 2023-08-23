/*============================================================================*/
/* XF_Optimise_sort_limit_All_Levels.cpp -- Optimise Onetime Calculations              */
/*============================================================================*/

#include "XF_Optimise_sort_limit_All_Levels.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_sort_limit_All_Levels, CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS)
__aNewFunction(XF_Optimise_sort_limit_All_Levels, CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS)
__vDeleteFunction(XF_Optimise_sort_limit_All_Levels, CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS)
__aFooterFunction(XF_Optimise_sort_limit_All_Levels)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_sort_limit_All_Levels::XF_Optimise_sort_limit_All_Levels(CardIter* pCI)
: XF_TRANSFORM(CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_sort_limit_All_Levels::~XF_Optimise_sort_limit_All_Levels(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_sort_limit_All_Levels::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_sort_limit_All_Levels* pXF_Optimise_sort_limit_All_Levels = new XF_Optimise_sort_limit_All_Levels(pCI);

  pXF_Optimise_sort_limit_All_Levels->bOptimise();

  delete pXF_Optimise_sort_limit_All_Levels;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_sort_limit_All_Levels::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_OPTIMISE_ONETIME_CALC, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();

  m_pCI->pBoard()->pState()->vInsert(aOT[OT_SORT_LIMIT_ALL_LEVELS]);

  m_pCI->vEndTransform();

  vValidate();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
