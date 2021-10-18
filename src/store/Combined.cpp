/*============================================================================*/
/* Combined.cpp -- Tub-Based Crw Combined using Dynamic Code                  */
/*============================================================================*/

#include "BASE.h"

#include "Combined.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Combined,CN_COMBINED)
__aNewFunction(Combined,CN_COMBINED)
__vDeleteFunction(Combined,CN_COMBINED)
__aFooterFunction(Combined)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Combined::Combined(void)
: Object(CN_COMBINED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);
  
  m_xCrwTubInflate.m_pTub       = nullptr;
  m_xCrwTubInflate.m_aHeadLine0 = nullptr;
  m_xCrwTubInflate.m_iHeadSize  = 0;
  m_xCrwTubInflate.m_nRows      = 0;
  m_xCrwTubInflate.m_nCols      = 0;
  m_xCrwTubInflate.m_eStrInflateRule = SI_NONE;
  m_xCrwTubInflate.m_bTrace     = false;

  _checkInitialised_(CN_COMBINED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Combined::~Combined(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair tubby                                                               */
/*----------------------------------------------------------------------------*/
void Combined::vRepair(Object* pObject)
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
CrwTubInflate_T* Combined::aCrwTubInflate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);

  return &m_xCrwTubInflate;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
