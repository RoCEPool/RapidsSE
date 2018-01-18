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
ItemBuffer* DBobjects::_pBufferAbstract(void* aAddr)
{
  ItemBuffer* pBufferAbstract = reinterpret_cast<ItemBuffer*>(aAddr);
  
  RASSERT(    !pBufferAbstract
           || CN_ENTRYBUFFER == pBufferAbstract->eClass()
           || CN_RESULTBUFFER == pBufferAbstract->eClass()
          );
  
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
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
Item* ItemBuffer::pAllotItem(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_);
  
  char* aStart = m_xWsp.aArea();
  
  return aStart;
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
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
i8 ItemBuffer::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nItems;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
