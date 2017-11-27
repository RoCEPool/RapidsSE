/*============================================================================*/
/* LoadMap.hpp -- Load Map                                                    */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoadMap* DBobjects::_pSeekMap(void* aAddr)
{
  LoadMap* pLoadMap = reinterpret_cast<LoadMap*>(aAddr);
  
  RASSERT( !pLoadMap || CN_LOADMAP == pLoadMap->eClass() );
  
  return pLoadMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadMap::operator new(size_t iSize)
{
  RASSERT(iSize);

  Object* pObject = Heap::pAllocate(CN_LOADMAP);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void LoadMap::operator delete(void* pLoadMapAddress)
{
  Object* pObject = static_cast<Object*>(pLoadMapAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadMap::LoadMap(void)
: Object(CN_LOADMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_iEntries = 0;
  m_bDone    = false;
  m_zLock.vInitLock(LK_LOAD_MAP);
  m_iNext    = 0;
  for (i8 i_Entry = 0; i_Entry < C_LOADMAP_ENTRIES; i_Entry++)
    m_iEolOffset[i_Entry] = -1;

  //m_xChunkFile
  m_iChunkNext = 0;
  _memZero_(m_xBlockTest,C_LOADMAP_BLOCK,CN_LOADMAP);
  m_iChunkTell = 0;
  
  _checkInitialised_(CN_LOADMAP);
  
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
void LoadMap::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_LOADMAP);
  ASSERT(pObject->eClass() == CN_LOADMAP);
  
  LoadMap* pSeekMap = reinterpret_cast<LoadMap*>(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set file path                                                              */
/*----------------------------------------------------------------------------*/
void LoadMap::vFilePath(StrVal& sPath)
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
  
  i8 iTestSeek = m_iChunkNext + C_LOADCHUNK_LENGTH - C_LOADMAP_BLOCK;
  
  if (iTestSeek > m_iChunkTell) {
    iTestSeek = m_iChunkTell - 1;
  }
  
  m_xChunkFile.vSeek(iTestSeek);
  
  i8 iBytesRead = m_xChunkFile.iReadChunk(m_xBlockTest,C_LOADMAP_BLOCK);
  
  const char *z = &m_xBlockTest[iBytesRead - 1];
  
  bool bFoundEol = StrLib::bBackward(&z,m_xBlockTest,'\n');
  RASSERT(bFoundEol);
  
  i8 nBytes = Base::nSub((void*)z,m_xBlockTest) + 1;
  
  i8 iEolOffset = iTestSeek + nBytes - 1;
  //vTrace("EolOffset %lld\n"_Stz,iEolOffset);
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
  i8 iChunk;
  i8 nChunks;
  
  while (bNextChunk(iSeek,nBytes,iChunk,nChunks)) {
    
    if (gpAttachMap->m_bProgress) {
      if (0 == iChunk % 1000) {
        vInfo("chunk %lld of %lld        \r"_Stz,iChunk,nChunks);
        fflush(stdout);
      }
    }
    
    //vTrace("Test Seek %lld Bytes %lld\n"_Stz,iSeek,nBytes);
    
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

  m_zLock.vGrabWrite();
  
  ++gpAttachMap->m_xLocks.m_zWorkUnits[LK_LOAD_MAP];
  
  m_iEolOffset[m_iEntries++] = iEolOffset;

  m_zLock.vDropWrite();
  
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
  
  //vTrace("LoadMap closed with %lld entries\n"_Stz,m_iEntries);
}

/*----------------------------------------------------------------------------*/
/* subordinate takes next read-chunk                                          */
/*----------------------------------------------------------------------------*/
bool LoadMap::bNextChunk(i8& iSeek, i8& nBytes, i8& iChunk, i8& nChunks)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);

  // is there another entry available yet?
  if (m_iNext == m_iEntries) {
    return false;
  }
  
  // subordinates compete for next entries
  m_zLock.vGrabWrite();
 
  // is there another entry available yet?
  if (m_iNext == m_iEntries) {
    m_zLock.vDropWrite();
    return false;
  }
  ASSERT(m_iNext < m_iEntries);
  
  // - a seekmap holds ending offsets only

  iChunk  = m_iNext;
  nChunks = m_iEntries;
  
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
/* stats                                                                      */
/*----------------------------------------------------------------------------*/
void LoadMap::vStats(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  vTrace("LoadMap: lock writes %lld write collisions %lld\n"_Stz,
         m_zLock.iWriteGrabs(),
         m_zLock.iWriteCollisions());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* LoadMap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  Footer* aFooter = &m_xFooter;
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
