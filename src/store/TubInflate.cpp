/*============================================================================*/
/* TubInflate.cpp -- Tub-Based Inflate using Dynamic Code                     */
/*============================================================================*/

#include "BASE.h"

#include "TubInflate.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,TubInflate,CN_TUBINFLATE)
__aNewFunction(TubInflate,CN_TUBINFLATE)
__vDeleteFunction(TubInflate,CN_TUBINFLATE)
__aFooterFunction(TubInflate)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TubInflate::TubInflate(void)
: Object(CN_TUBINFLATE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUBINFLATE,_FOOTER_);
  
  m_xCrwTubInflate.m_aHeadLine0 = nullptr;
  m_xCrwTubInflate.m_iHeadSize  = 0;
  m_xCrwTubInflate.m_nRows      = 0;
  m_xCrwTubInflate.m_nCols      = 0;
  m_xCrwTubInflate.eStrInflateRule = SI_NONE;
  m_xCrwTubInflate.m_aTub       = nullptr;

  _checkInitialised_(CN_TUBINFLATE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TUBINFLATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TubInflate::~TubInflate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUBINFLATE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair tubby                                                               */
/*----------------------------------------------------------------------------*/
void TubInflate::vRepair(Object* pObject)
{
  _static_
  
  UNUSED(pObject);
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get m_xCrwTubInflate                                                       */
/*----------------------------------------------------------------------------*/
CrwTubInflate_T* TubInflate::aCrwTubInflate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUBINFLATE,_FOOTER_);

  return &m_xCrwTubInflate;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
