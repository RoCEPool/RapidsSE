/*============================================================================*/
/* LoadMap.hpp -- Load Map                                                    */
/*============================================================================*/

#include "Chunks.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoadMap* Chunks::_pLoadMap(void* aAddr)
{
  LoadMap* pLoadMap = reinterpret_cast<LoadMap*>(aAddr);
  
  RASSERT_TYPE(pLoadMap,CN_LOADMAP);
  
  return pLoadMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadMap::operator new(size_t)
{
  Object* pObject = HeapMem::pAllocate(CN_LOADMAP);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void LoadMap::operator delete(void* pLoadMapAddress)
{
  Object* pObject = static_cast<Object*>(pLoadMapAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadMap::LoadMap(Stz* aFilePath)
: Object(CN_LOADMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_iEntries = 0;
  m_bDone    = false;
  
  m_sFilePath = aFilePath;
  
  m_zLock.vInitLock(LK_LOAD_MAP);
  m_iNext    = 0;
  for (i8 i_Entry = 0; i_Entry < C_LOADMAP_ENTRIES; i_Entry++)
    m_iEolOffset[i_Entry] = -1;

  //m_oChunkFile
  m_iNextChunk = 0;
  _memZero_(m_xBlockTest,C_LOADMAP_BLOCK,CN_LOADMAP);
  m_iSeekEof = 0;
  
  _checkInitialised_(CN_LOADMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoadMap::~LoadMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
}

/*----------------------------------------------------------------------------*/
/* dump seekmap                                                               */
/*----------------------------------------------------------------------------*/
void LoadMap::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  LoadMap* pSeekMap = _pLoadMap(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void LoadMap::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  m_oChunkFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* dump load map                                                              */
/*----------------------------------------------------------------------------*/
void LoadMap::vDumpMap(void)
{
  for (i8 i_Entry = 0; i_Entry < m_iEntries; i_Entry++) {
    if (i_Entry && 0 == i_Entry % 8)
      vTrace("\n"_Stz);
    vTrace("%ld "_Stz,m_iEolOffset[i_Entry]);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* fetch next chunk from file                                                 */
/*----------------------------------------------------------------------------*/
bool LoadMap::bFindChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  if (m_iNextChunk >= m_iSeekEof) {
    vDone(true);
    //vChunkTest();
    //m_oChunkFile.vClose();
    return false;
  }
  
  i8 iTestSeek = m_iNextChunk + C_LOAD_CHUNK - C_LOADMAP_BLOCK;
  
  if (iTestSeek > m_iSeekEof) {
    iTestSeek = m_iSeekEof - 1;
    vEolOffset(iTestSeek);
    vDone(true);
    //m_oChunkFile.vClose();
    return false;
  }
  
  m_oChunkFile.vSeek(iTestSeek);
  
  i8 iBytesRead = m_oChunkFile.iReadBytes(m_xBlockTest,C_LOADMAP_BLOCK);
  
  const char *z = &m_xBlockTest[iBytesRead - 1];
  
  bool bFoundEol = StrLib::bBackward(&z,m_xBlockTest,'\n');
  if (!bFoundEol)
    bFoundEol = StrLib::bBackward(&z,m_xBlockTest,'\r');
  
  RASSERT(bFoundEol);
  
  i8 nBytes = Base::nSub((void*)z,m_xBlockTest) + 1;
  
  i8 iEolOffset = iTestSeek + nBytes - 1;
  //vTrace("EolOffset %lld\n"_Stz,iEolOffset);
  vEolOffset(iEolOffset);
  
  m_iNextChunk = iEolOffset + 1;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool LoadMap::bOpenFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  bool bOpened = m_oChunkFile.bOpen(m_sFilePath,C_OPEN_READ);
  if (! bOpened)
    return false;
  
  m_oChunkFile.vSeekEnd();
  m_iSeekEof = m_oChunkFile.iTell();
  
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
      m_oChunkFile.vSeek(iSeek - 1);
      i8 iBytesRead = m_oChunkFile.iReadBytes(m_xBlockTest,1);
      ASSERT(1 == iBytesRead);
      ASSERT('\n' == m_xBlockTest[0]);
    }
    
    m_oChunkFile.vSeek(iSeek + nBytes - 1);
    i8 iBytesRead = m_oChunkFile.iReadBytes(m_xBlockTest,1);
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
  
  ++gpAttachMap->m_oLockTracker.m_ziWorkUnits[LK_LOAD_MAP];
  
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
bool LoadMap::bNextChunk(i8& iSeek, i8& nBytes, i8& i_Chunk, i8& nChunks)
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
  
  // a subsequent subordinate calling this func will get the next entry
  // save m_iNext before incrementing
  i_Chunk  = m_iNext++;
  
  // now ok to drop the lock
  m_zLock.vDropWrite();
  
  // - a seekmap holds ending offsets only
  nChunks = m_iEntries;
  
  // calculate starting seek offset of next read-chunk
  if (0 == i_Chunk)
    iSeek = 0;
  else
    iSeek = m_iEolOffset[i_Chunk - 1] + 1;
  
  // calculate the number of bytes in the read-chunk
  i8 iEnd = m_iEolOffset[i_Chunk];
  nBytes  = iEnd - iSeek + 1;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get next chunk information                                                 */
/*----------------------------------------------------------------------------*/
bool LoadMap::bNextChunkInfo(i8& i_Seek, i8& nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);

  i8 i_Chunk;
  i8 nChunks;
  
  // get information about next chunk
  for (;;) {
    
    bool bMoreChunks = bNextChunk(i_Seek,nBytes,i_Chunk,nChunks);
    
    if (m_bDone && ! bMoreChunks)
      return false;
    
    if (bMoreChunks)
      break;
    
    //vTrace("Waiting for map\n"_Stz);
  }
  
  if (gpAttachMap->m_bProgress) {
    if (0 == i_Chunk % 100) {
      vInfo("read chunk %lld of %lld        \r"_Stz,i_Chunk,nChunks);
      fflush(stdout);
    }
  }
  
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
