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
  
  m_nEntries = 0;
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
/* allot an entry for a row in this buffer   //TEMPORARY!!!                   */
/*----------------------------------------------------------------------------*/
Entry* BufferAbstract::pAllotEntryBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes) {
    return nullptr;
  }
  
  Entry* pEntry = reinterpret_cast<Entry*>(m_xWsp.aAllot(nBytes));
  new(pEntry) Entry(CN_ENTRY,nBytes,0);
  
  m_nEntries++;
  
  return pEntry;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
