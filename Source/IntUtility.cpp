/*============================================================================*/
/* IntUtility.cpp -- Int Utiliity                                             */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Utilities,IntUtility,CN_INTUTILITY)
__aNewFunction(IntUtility,CN_INTUTILITY)
__vDeleteFunction(IntUtility,CN_INTUTILITY)
__aFooterFunction(IntUtility)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IntUtility::IntUtility(void)
: Object(CN_INTUTILITY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
    
  m_pIntAry = nullptr;
  
  _checkInitialised_(CN_INTUTILITY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IntUtility::IntUtility(i8 nInts)
: Object(CN_INTUTILITY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
    
  m_pIntAry = new ArrayNL_t<CN_INTUTILITY,i8,C_64KB>;
  
  _checkInitialised_(CN_INTUTILITY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IntUtility::~IntUtility(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
  
  delete m_pIntAry;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set as an ascending set of integers                                        */
/*----------------------------------------------------------------------------*/
void IntUtility::vAscending(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
  

  i8 nMaxItems = m_pIntAry->nMaxItems();
  
  for (i8 i_Item = 0; i_Item < nMaxItems; i_Item++)
    vSet(i_Item,i_Item);
}

/*----------------------------------------------------------------------------*/
/* set as an descending set of integers                                       */
/*----------------------------------------------------------------------------*/
void IntUtility::vDescending(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
  

  i8 nMaxItems = m_pIntAry->nMaxItems();
  
  i8 iInt = nMaxItems - 1;
  for (i8 i_Item = 0; i_Item < nMaxItems; i_Item++)
    vSet(i_Item,iInt--);
}

/*----------------------------------------------------------------------------*/
/* randomise the set                                                          */
/*----------------------------------------------------------------------------*/
void IntUtility::vRandomise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);

  i8 nMaxItems = m_pIntAry->nMaxItems();
  
  for (i8 i_Times = 0; i_Times < nMaxItems; i_Times++) {
    
    i8 i_Left  = RndLib::iInteger(nMaxItems);
    i8 i_Right = RndLib::iInteger(nMaxItems);

    i8 iVal_left  = iGet(i_Left);
    i8 iVal_right = iGet(i_Right);

    vSet(i_Left,iVal_right);
    vSet(i_Right,iVal_left);
  }
}

/*----------------------------------------------------------------------------*/
/* get an int                                                                 */
/*----------------------------------------------------------------------------*/
i8 IntUtility::iGet(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
  
  return m_pIntAry->xGet(i_Item);
}

/*----------------------------------------------------------------------------*/
/* get number of ints                                                         */
/*----------------------------------------------------------------------------*/
i8 IntUtility::nInts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);

  i8 nInts = m_pIntAry->nItems();
  
  return nInts;
}

/*----------------------------------------------------------------------------*/
/* set an int                                                                 */
/*----------------------------------------------------------------------------*/
void IntUtility::vSet(i8 i_Item, i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTUTILITY,_FOOTER_);
  
  m_pIntAry->vSet(i_Item,iInt);
  if (i_Item >= m_pIntAry->nItems())
    m_pIntAry->vItems(i_Item+1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
