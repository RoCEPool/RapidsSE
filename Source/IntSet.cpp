/*============================================================================*/
/* IntSet.cpp -- Int Set                                                      */
/*============================================================================*/

#include "Utilities.h"

int Utilities::iSortCompare_ascend(const void * a, const void * b)
{
  return (int) ( *(i8*)a - *(i8*)b );
}
   
int Utilities::iSortCompare_descend(const void * a, const void * b)
{
  return (int) ( *(i8*)b - *(i8*)a );
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Utilities,IntSet,CN_INTSET)
__aNewFunction(IntSet,CN_INTSET)
__vDeleteFunction(IntSet,CN_INTSET)
__aFooterFunction(IntSet)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IntSet::IntSet(void)
: Object(CN_INTSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
    
  m_pIntAry = nullptr;
  
  _checkInitialised_(CN_INTSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IntSet::IntSet(i8 nInts)
: Object(CN_INTSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
    
  m_pIntAry = new ArrayNL_t<CN_INTSET,i8,C_64KB>;
  m_pIntAry->vItems(nInts);
  
  _checkInitialised_(CN_INTSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IntSet::~IntSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
  delete m_pIntAry;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set as an ascending set of integers                                        */
/*----------------------------------------------------------------------------*/
void IntSet::vAscending(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  

  i8 nItems = m_pIntAry->nItems();
  
  for (i8 i_Item = 0; i_Item < nItems; i_Item++)
    vSet(i_Item,i_Item);
}

/*----------------------------------------------------------------------------*/
/* set as an descending set of integers                                       */
/*----------------------------------------------------------------------------*/
void IntSet::vDescending(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  

  i8 nItems = m_pIntAry->nItems();
  
  i8 iInt = nItems - 1;
  for (i8 i_Item = 0; i_Item < nItems; i_Item++)
    vSet(i_Item,iInt--);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void IntSet::vPrint(bool bNewLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
  i8 nItems = m_pIntAry->nItems();
  
  for (i8 i_Item = 0; i_Item < nItems; i_Item++)
    if (bNewLine)
      vTrace("%lld\n"_Stz,iGet(i_Item));
    else
      vTrace("%lld "_Stz,iGet(i_Item));

  if (! bNewLine)
    vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* randomise the set                                                          */
/*----------------------------------------------------------------------------*/
void IntSet::vRandomise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);

  i8 nItems = m_pIntAry->nItems();
  
  for (i8 i_Times = 0; i_Times < nItems; i_Times++) {
    
    i8 i_Left  = RndLib::iInteger(nItems);
    i8 i_Right = RndLib::iInteger(nItems);

    i8 iVal_left  = iGet(i_Left);
    i8 iVal_right = iGet(i_Right);

    vSet(i_Left,iVal_right);
    vSet(i_Right,iVal_left);
  }
}

/*----------------------------------------------------------------------------*/
/* populate with a set of random integers                                     */
/*----------------------------------------------------------------------------*/
void IntSet::vRandoms(i8 iMin, i8 iMax)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
  i8 nItems = m_pIntAry->nItems();

  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
    i8 iRndInt = RndLib::iInteger(iMin,iMax);
    
    vSet(i_Item,iRndInt);
  }
}

/*----------------------------------------------------------------------------*/
/* populate with a set of unique random integers                              */
/*----------------------------------------------------------------------------*/
void IntSet::vRandomsUnique(i8 iMin, i8 iMax)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
  i8 iRndInt = 0;
  
  i8 nItems = m_pIntAry->nItems();

  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
    
    do
      iRndInt = RndLib::iInteger(iMin,iMax);
    while (bHas(iRndInt));
    
    vSet(i_Item,iRndInt);
  }
}

/*----------------------------------------------------------------------------*/
/* sort the set                                                               */
/*----------------------------------------------------------------------------*/
void IntSet::vSort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
   qsort(m_pIntAry->aItemArea(),nInts(),sizeof(i8),Utilities::iSortCompare_ascend);
}

/*----------------------------------------------------------------------------*/
/* sort the set (descending)                                                  */
/*----------------------------------------------------------------------------*/
void IntSet::vSortDescend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
   qsort(m_pIntAry->aItemArea(),nInts(),sizeof(i8),Utilities::iSortCompare_descend);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is int in this set?                                                        */
/*----------------------------------------------------------------------------*/
bool IntSet::bHas(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
 if (C_UNKNOWN == iFind(iInt))
   return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get an int                                                                 */
/*----------------------------------------------------------------------------*/
i8 IntSet::iFind(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
  i8 nInts = m_pIntAry->nItems();

  for (i8 i_Item = 0; i_Item < nInts; i_Item++)
    if (iInt == iGet(i_Item))
      return i_Item;
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get an int                                                                 */
/*----------------------------------------------------------------------------*/
i8 IntSet::iGet(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
  return m_pIntAry->xGet(i_Item);
}

/*----------------------------------------------------------------------------*/
/* get number of ints                                                         */
/*----------------------------------------------------------------------------*/
i8 IntSet::nInts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);

  i8 nInts = m_pIntAry->nItems();
  
  return nInts;
}

/*----------------------------------------------------------------------------*/
/* set an int                                                                 */
/*----------------------------------------------------------------------------*/
void IntSet::vSet(i8 i_Item, i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTSET,_FOOTER_);
  
  m_pIntAry->vSet(i_Item,iInt);
  if (i_Item >= m_pIntAry->nItems())
    m_pIntAry->vItems(i_Item+1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
