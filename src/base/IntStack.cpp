/*============================================================================*/
/* IntStack.cpp -- Int Stack                                                  */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,IntStack,CN_INTSTACK)
__aNewFunction(IntStack,CN_INTSTACK)
__vDeleteFunction(IntStack,CN_INTSTACK)
__aFooterFunction(IntStack)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IntStack::IntStack(void)
: Object(CN_INTSTACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSTACK,_FOOTER_);
      
  m_pIntAry = new ArrayNL_t<CN_INTSTACK,i8,C_64KB>;;
  
  _checkInitialised_(CN_INTSTACK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INTSTACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IntStack::~IntStack(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSTACK,_FOOTER_);
  
  delete m_pIntAry;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the int stack                                                      */
/*----------------------------------------------------------------------------*/
void IntStack::vRepair(Object* pObject)
{
  _static_

  IntStack* pIntStack = _pIntStack(pObject);

  // delete the intStack
  delete pIntStack;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get number of ints                                                         */
/*----------------------------------------------------------------------------*/
i8 IntStack::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSTACK,_FOOTER_);

  i8 nItems = m_pIntAry->nItems();
  
  return nItems;
}

/*----------------------------------------------------------------------------*/
/* peek at int                                                                */
/*----------------------------------------------------------------------------*/
i8 IntStack::iPeek(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSTACK,_FOOTER_);
  
  i8 iInt = m_pIntAry->xGet(i_Item);

  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop top int                                                                */
/*----------------------------------------------------------------------------*/
i8 IntStack::iPop()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSTACK,_FOOTER_);
  
  i8 nItems = m_pIntAry->nItems();
  ASSERT(nItems > 0);

  i8 iInt = m_pIntAry->xGet(nItems - 1);
  
  m_pIntAry->vDecItems();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void IntStack::vPrint(bool bNewLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSTACK,_FOOTER_);
  
  i8 nItems = m_pIntAry->nItems();
  
  for (i8 i_Item = 0; i_Item < nItems; i_Item++)
    if (bNewLine)
      vTrace("%lld\n"_Stz,iPeek(i_Item));
    else
      vTrace("%lld "_Stz,iPeek(i_Item));

  if (! bNewLine)
    vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* push an int                                                                */
/*----------------------------------------------------------------------------*/
void IntStack::vPush(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSTACK,_FOOTER_);
  
  i8 nItems = m_pIntAry->nItems();
  
  m_pIntAry->vSet(nItems,iInt);

  m_pIntAry->vIncItems();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
