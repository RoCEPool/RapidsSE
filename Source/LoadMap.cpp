/*============================================================================*/
/* LoadMap.hpp -- Load Map                                                    */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoadMap* DBobjects::_pSeekMap(void* pAddr)
{
  LoadMap* pSeekMap = reinterpret_cast<LoadMap*>(pAddr);
  
  return pSeekMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadMap::operator new(size_t iSize)
{
  RASSERT(iSize);

  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void LoadMap::operator delete(void* pSkipMapAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pSkipMapAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadMap::LoadMap(void)
: Buffer(CN_LOADMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_iEntries = 0;
  m_bDone    = false;
  //m_zLock
  m_iNext    = 0;
  for (i8 iEntry = 0; iEntry < C_LOADMAP_ENTRIES; iEntry++)
    m_iEolOffset[iEntry] = -1;

  //m_xChunkFile
  m_iChunkNext = 0;
  Memory::vZero(m_xBlockTest,C_LOADMAP_BLOCK);
  m_iChunkTell = 0;
  
  _checkInitialised_(sizeof(CN_LOADMAP));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoadMap::~LoadMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
}

/*----------------------------------------------------------------------------*/
/* dump seekmap                                                               */
/*----------------------------------------------------------------------------*/
void LoadMap::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_LOADMAP);
  ASSERT(pObject->eClass() == CN_LOADMAP);
  
  LoadMap* pSeekMap = reinterpret_cast<LoadMap*>(pObject);
  
  vTrace("%sSeekMap %p:\n", pTab, pSeekMap);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set file path                                                              */
/*----------------------------------------------------------------------------*/
void LoadMap::vFilePath(s& sPath)
{
  m_xChunkFile.vPath(sPath);
}

/*----------------------------------------------------------------------------*/
/* fetch next chunk from file                                                 */
/*----------------------------------------------------------------------------*/
bool LoadMap::bFindChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  if (! m_xChunkFile.bIsOpen()) {
    bool b = m_xChunkFile.bOpen(C_OPEN_READ);
    if (! b)
      return false;
    
    m_xChunkFile.vSeekEnd();
    m_iChunkTell = m_xChunkFile.iTell();
  }
  
  if (m_iChunkNext >= m_iChunkTell) {
    vDone(true);
    //vChunkTest();
    m_xChunkFile.vClose();
    return false;
  }
  
  i8 iTestSeek = m_iChunkNext + C_LOADCHUNK_SIZE - C_LOADMAP_BLOCK;
  
  if (iTestSeek > m_iChunkTell) {
    iTestSeek = m_iChunkTell - 1;
  }
  
  m_xChunkFile.vSeek(iTestSeek);
  
  i8 iBytesRead = m_xChunkFile.iReadChunk(m_xBlockTest,C_LOADMAP_BLOCK);
  
  const char *z = &m_xBlockTest[iBytesRead - 1];
  
  bool bFoundEol = String::bBackward(&z,m_xBlockTest,'\n');
  RASSERT(bFoundEol);
  
  i8 nBytes = Magic::iSub((void*)z,m_xBlockTest) + 1;
  
  i8 iEolOffset = iTestSeek + nBytes - 1;
  //vTrace("EolOffset %lld\n",iEolOffset);
  vEolOffset(iEolOffset);
  
  m_iChunkNext = iEolOffset + 1;
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* test if we chunked correctly                                               */
/*----------------------------------------------------------------------------*/
void LoadMap::vChunkTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  i8 iSeek;
  i8 nBytes;
  
  while (bNextChunk(iSeek,nBytes)) {
    
    //vTrace("Test Seek %lld Bytes %lld\n",iSeek,nBytes);
    
    if (iSeek) {
      m_xChunkFile.vSeek(iSeek - 1);
      i8 iBytesRead = m_xChunkFile.iReadChunk(m_xBlockTest,1);
      ASSERT(1 == iBytesRead);
      ASSERT('\n' == m_xBlockTest[0]);
    }
    
    m_xChunkFile.vSeek(iSeek + nBytes - 1);
    i8 iBytesRead = m_xChunkFile.iReadChunk(m_xBlockTest,1);
    ASSERT(1 == iBytesRead);
    ASSERT('\n' == m_xBlockTest[0]);
    
  }
}








/*----------------------------------------------------------------------------*/
/* parent adds the offset of the end of a read-chunk                          */
/*----------------------------------------------------------------------------*/
void LoadMap::vEolOffset(i8 iEolOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);

  m_iEolOffset[m_iEntries++] = iEolOffset;
  
  RASSERT(m_iEntries < C_LOADMAP_ENTRIES);
}

/*----------------------------------------------------------------------------*/
/* has the parent completed this map yet?                                     */
/*----------------------------------------------------------------------------*/
bool LoadMap::bDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  return m_bDone;
}

/*----------------------------------------------------------------------------*/
/* the parent has completed this map                                          */
/*----------------------------------------------------------------------------*/
void LoadMap::vDone(bool bDone)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_bDone = bDone;
  
  vTrace("LoadMap closed with %lld entries\n",m_iEntries);
}

/*----------------------------------------------------------------------------*/
/* subordinate takes next read-chunk                                          */
/*----------------------------------------------------------------------------*/
bool LoadMap::bNextChunk(i8& iSeek, i8& nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);

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
    iSeek = m_iEolOffset[m_iNext - 1] + 1;
  
  // calculate the number of bytes in the read-chunk
  i8 iEnd = m_iEolOffset[m_iNext];
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
Footer* LoadMap::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  Footer* pFooter = &m_xFooter;
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/