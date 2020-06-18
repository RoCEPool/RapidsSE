/*============================================================================*/
/* IntUtility.cpp -- Int Utiliity                                             */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Utilities,IntUtility,CN_ARRANGE)
__aNewFunction(IntUtility,CN_ARRANGE)
__vDeleteFunction(IntUtility,CN_ARRANGE)
__aFooterFunction(IntUtility)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IntUtility::IntUtility(void)
: Object(CN_ARRANGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
    
  m_pIntAry = nullptr;
  
  _checkInitialised_(CN_ARRANGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IntUtility::IntUtility(i8 nInts)
: Object(CN_ARRANGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
    
  m_pIntAry = new ArrayNL_t<CN_INTUTILITY,i8,C_64KB>;
  
  _checkInitialised_(CN_ARRANGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IntUtility::~IntUtility(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  delete m_pIntAry;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get an int                                                                 */
/*----------------------------------------------------------------------------*/
i8 IntUtility::iGet(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  return m_pIntAry->xGet(i_Item);
}

/*----------------------------------------------------------------------------*/
/* get number of ints                                                         */
/*----------------------------------------------------------------------------*/
i8 IntUtility::nInts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);

  i8 nInts = m_pIntAry->nItems();
  
  return nInts;
}

/*----------------------------------------------------------------------------*/
/* set an int                                                                 */
/*----------------------------------------------------------------------------*/
void IntUtility::vSet(i8 i_Item, i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  m_pIntAry->vSet(i_Item,iInt);
  if (i_Item >= m_pIntAry->nItems())
    m_pIntAry->vItems(i_Item+1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
