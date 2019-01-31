/*============================================================================*/
/* ItemBuffer.cpp -- Item Buffer                                              */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ItemBuffer* DBobjects::_pItemBuffer(void* aAddr)
{
  ItemBuffer* pBufferAbstract = reinterpret_cast<ItemBuffer*>(aAddr);
  
  return pBufferAbstract;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ItemBuffer::ItemBuffer(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_nItems = 0;
  //m_xWsp
  
#ifdef WITH_PREPOST_CHECKING
  m_xWsp.vZero(eClassNo);
#endif
  
  vSetWilderness();
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //  _checkInitialised_(eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ItemBuffer::~ItemBuffer(void) noexcept(false)
{
  //_precondition_(_ADDRESS_);
}

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
Item* ItemBuffer::aAllotItem(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Item* aItem = reinterpret_cast<Item*>(aAllotSpace(nBytes));
  
  if (! aItem)
    return nullptr;
  
  m_nItems++;
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* allot space for nBytes in this buffer                                      */
/*----------------------------------------------------------------------------*/
char* ItemBuffer::aAllotSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  char* aSpace = m_xWsp.aAllot(nBytes);
  
  vSetWilderness();
  
  return aSpace;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* ItemBuffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  char* aStart = m_xWsp.aArea();
  
  return aStart;
}

/*----------------------------------------------------------------------------*/
/* decrement number of items in buffer                                        */
/*----------------------------------------------------------------------------*/
void ItemBuffer::vDecItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
Item* ItemBuffer::aFirstItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if ( m_nItems == 0 )
    return nullptr;
  
  Item* aFirst = reinterpret_cast<Item*>(m_xWsp.aArea());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 ItemBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* give space back to this buffer                                             */
/*----------------------------------------------------------------------------*/
void ItemBuffer::vGiveSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_xWsp.vGive(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
i8 ItemBuffer::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
void ItemBuffer::vItems(i8 nItems)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
Item* ItemBuffer::aNextItem(Item* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = aItem->nSizeItem();
  
  Item* aNext = aAdd<Item*>(aItem,nBytes);
  
  if ((char*)aNext >= (char*)m_xWsp.aLatter())
    return nullptr;
    
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* reset used space                                                           */
/*----------------------------------------------------------------------------*/
void ItemBuffer::vResetUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_xWsp.vUsed(0);
  
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* reset used space                                                           */
/*----------------------------------------------------------------------------*/
void ItemBuffer::vSetUsed(Item* aUptil)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nUsedBytes = nSub(aUptil,aDataArea());
  
  m_xWsp.vUsed(nUsedBytes);
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given item              */
/*----------------------------------------------------------------------------*/
i4 ItemBuffer::iToOffset(Item* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i4 iOffset = static_cast<i4>(nSub(aItem,m_xWsp.aArea()));
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 ItemBuffer::nUsedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set wilderness marker                                                      */
/*----------------------------------------------------------------------------*/
void ItemBuffer::vSetWilderness(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (nFreeBytes() < (i8)sizeof(Wilderness))
    return;
  
  Wilderness* aWilderness = reinterpret_cast<Wilderness*>(m_xWsp.aLatter());
  
  new(aWilderness) Wilderness;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
