/*============================================================================*/
/* SeekMap.hpp -- Seek Map                                                    */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SeekMap* DBobjects::_pSeekMap(void* pAddr)
{
  SeekMap* pSeekMap = reinterpret_cast<SeekMap*>(pAddr);
  
  return pSeekMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SeekMap::operator new(size_t iSize)
{
  RASSERT(iSize);

  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void SeekMap::operator delete(void* pSkipMapAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pSkipMapAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SeekMap::SeekMap(void)
: Buffer(CN_SEEKMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKMAP);
  
  m_iEntries = 0;
  m_bDone    = false;
  //m_zLock
  m_iNext    = 0;
  for (i8 iEntry = 0; iEntry < C_SEEKMAP_ENTRIES; iEntry++)
    m_iEnd[iEntry] = -1;

  _checkInitialised_(sizeof(CN_SEEKMAP));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEEKMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump seekmap                                                               */
/*----------------------------------------------------------------------------*/
void SeekMap::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_SEEKMAP);
  ASSERT(pObject->eClass() == CN_SEEKMAP);
  
  SeekMap* pSeekMap = reinterpret_cast<SeekMap*>(pObject);
  
  vTrace("%sSeekMap %p:\n", pTab, pSeekMap);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parent adds the offset of the '\n' which ends a read-chunk                 */
/*----------------------------------------------------------------------------*/
void SeekMap::vAdd(i8 iEndOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKMAP);

  m_iEnd[m_iEntries++] = iEndOffset;
}

/*----------------------------------------------------------------------------*/
/* has the parent completed this map yet?                                     */
/*----------------------------------------------------------------------------*/
bool SeekMap::bDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKMAP);
  
  return m_bDone;
}

/*----------------------------------------------------------------------------*/
/* the parent has completed this map                                          */
/*----------------------------------------------------------------------------*/
void SeekMap::vDone(bool bDone)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKMAP);
  
  m_bDone = bDone;
}

/*----------------------------------------------------------------------------*/
/* subordinate takes next read-chunk                                          */
/*----------------------------------------------------------------------------*/
bool SeekMap::bNext(i8& iSeek, i8& nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKMAP);

  // subordinates compete for next entries
  m_zLock.vGrabWrite();
 
  // is there another entry available yet?
  if (m_iNext == m_iEntries) {
    m_zLock.vDropWrite();
    return false;
  }
  ASSERT(m_iNext < m_iEntries);
  
  // - a seekmap holds ending offsets only
  
  // calculate starting seek offset of next read-chunk
  if (0 == m_iNext)
    iSeek = 0;
  else
    iSeek = m_iEnd[m_iNext - 1] + 1;
  
  // calculate the number of bytes in the read-chunk
  i8 iEnd = m_iEnd[m_iNext];
  nBytes  = iEnd - iSeek + 1;
  
  // a subsequent subordinate calling this func will get the next entry
  m_iNext++;
  
  m_zLock.vDropWrite();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SeekMap::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKMAP);
  
  Footer* pFooter = &m_xFooter;
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/