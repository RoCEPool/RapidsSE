/*============================================================================*/
/* Inflator.cpp -- Tub-Based Crw Inflator using Dynamic Code                  */
/*============================================================================*/

#include "BASE.h"

#include "Inflator.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Inflator,CN_INFLATOR)
__aNewFunction(Inflator,CN_INFLATOR)
__vDeleteFunction(Inflator,CN_INFLATOR)
__aFooterFunction(Inflator)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Inflator::Inflator(void)
: Object(CN_INFLATOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFLATOR,_FOOTER_);
  
  m_xCrwTubInflate.m_aHeadLine0 = nullptr;
  m_xCrwTubInflate.m_iHeadSize  = 0;
  m_xCrwTubInflate.m_nRows      = 0;
  m_xCrwTubInflate.m_nCols      = 0;
  m_xCrwTubInflate.m_eStrInflateRule = SI_NONE;
  m_xCrwTubInflate.m_pTub       = nullptr;

  _checkInitialised_(CN_INFLATOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INFLATOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Inflator::~Inflator(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFLATOR,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair tubby                                                               */
/*----------------------------------------------------------------------------*/
void Inflator::vRepair(Object* pObject)
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
CrwTubInflate_T* Inflator::aCrwTubInflate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFLATOR,_FOOTER_);

  return &m_xCrwTubInflate;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
