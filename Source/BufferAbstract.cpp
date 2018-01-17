/*============================================================================*/
/* BufferAbstract.cpp -- Abstract Buffer                                      */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
BufferAbstract* DBobjects::_pBufferAbstract(void* aAddr)
{
  BufferAbstract* pBufferAbstract = reinterpret_cast<BufferAbstract*>(aAddr);
  
  RASSERT(    !pBufferAbstract
           || CN_ENTRYBUFFER == pBufferAbstract->eClass()
           || CN_RESULTBUFFER == pBufferAbstract->eClass()
          );
  
  return pBufferAbstract;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferAbstract::BufferAbstract(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_nItems = 0;
  //m_xWsp
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vZero(CN_ENTRYBUFFER);
#endif
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  _checkInitialised_(eClassNo);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferAbstract::~BufferAbstract(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
char* BufferAbstract::aAllotItem(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  char* aItem = m_xWsp.aAllot(nBytes);
  
  m_nItems++;
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in segment?                            */
/*----------------------------------------------------------------------------*/
i8 BufferAbstract::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
i8 BufferAbstract::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nItems;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
