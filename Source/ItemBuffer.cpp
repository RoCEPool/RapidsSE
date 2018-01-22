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
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vZero(eClassNo);
#endif
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //  _checkInitialised_(eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ItemBuffer::~ItemBuffer(void)
{
  //_precondition_(_ADDRESS_);
}

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
Item* ItemBuffer::pAllotItem(i8 nBytes)
{
  //_precondition_(_ADDRESS_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  Item* pItem = reinterpret_cast<Item*>(m_xWsp.aAllot(nBytes));
  
  m_nItems++;
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* ItemBuffer::aDataArea(void)
{
  //_precondition_(_ADDRESS_);
  
  char* aStart = m_xWsp.aArea();
  
  return aStart;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
Item* ItemBuffer::pFirstItem(void)
{
  //_precondition_(_ADDRESS_);
  
  if ( m_nItems == 0 )
    return nullptr;
  
  Item* pFirst = reinterpret_cast<Item*>(m_xWsp.aArea());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 ItemBuffer::nFreeBytes(void)
{
  //_precondition_(_ADDRESS_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
i8 ItemBuffer::nItems(void)
{
  //_precondition_(_ADDRESS_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
Item* ItemBuffer::pNextItem(Item* pCurrentItem)
{
  //_precondition_(_ADDRESS_);
  
  i8 nBytes = pCurrentItem->nSizeItem();
  
  Item* pNext = pAdd<Item*>(pCurrentItem,nBytes);
  
  if ((char*)pNext >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 ItemBuffer::nUsedBytes(void)
{
  //_precondition_(_ADDRESS_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
