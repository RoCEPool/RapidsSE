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
: FileChunk(CN_LOADMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_sFilePath = aFilePath;
  m_iChunkOffset = 0;
  m_iEofOffset = 0;
  m_bMappingDone = false;

  m_zEolLock.vInitLock(LK_LOAD_MAP);
  
  for (i8 i_Entry = 0; i_Entry < C_LOADMAP_ENTRIES; i_Entry++)
    m_iEolOffset[i_Entry] = -1;
  m_nEntries = 0;
  m_iNext = 0;

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
  //LoadMap* pSeekMap = _pLoadMap(pObject);
  
  //vTrace("\n"_Stz);
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
  
  FileChunk::vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* dump load map                                                              */
/*----------------------------------------------------------------------------*/
void LoadMap::vDumpMap(void)
{
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++) {
    if (i_Entry && 0 == i_Entry % 8)
      vTrace("\n"_Stz);
    vTrace("%ld "_Stz,m_iEolOffset[i_Entry]);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* map next chunk in file                                                     */
/*----------------------------------------------------------------------------*/
bool LoadMap::bMapChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  if (m_iChunkOffset >= m_iEofOffset) {
    vMappingDone(true);
    return false;
  }
  
  i8 iTestSeek = m_iChunkOffset + C_LOAD_CHUNK - C_LOADMAP_BLOCK;
  
  if (iTestSeek > m_iEofOffset) {
    iTestSeek = m_iEofOffset - 1;
    vEolOffset(iTestSeek);
    vMappingDone(true);
    return false;
  }
  
  m_oFile.vSeek(iTestSeek);
  
  char* aDst = aDataArea();
  
  i8 iBytesRead = m_oFile.iReadBytes(aDst,C_LOADMAP_BLOCK);
  
  const char *z = &aDst[iBytesRead - 1];
  
  bool bFoundEol = StrLib::bBackward(&z,aDst,'\n');
  if (!bFoundEol)
    bFoundEol = StrLib::bBackward(&z,aDst,'\r');
  
  RASSERT(bFoundEol);
  
  i8 nBytes = Base::nSub((void*)z,aDst) + 1;
  
  i8 iEolOffset = iTestSeek + nBytes - 1;
  vEolOffset(iEolOffset);
  
  m_iChunkOffset = iEolOffset + 1;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool LoadMap::bOpenFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  bool bOpened = bOpenRead(m_sFilePath);
  if (! bOpened)
    return false;
  
  m_oFile.vSeekEnd();
  m_iEofOffset = m_oFile.iTell();
  
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
  
  char* aDst = aDataArea();

  while (bNextChunk(iSeek,nBytes,iChunk,nChunks)) {
    
    if (gpAttachMap->m_bProgress) {
      if (0 == iChunk % 1000) {
        vInfo("chunk %lld of %lld        \r"_Stz,iChunk,nChunks);
        fflush(stdout);
      }
    }
    
    //vTrace("Test Seek %lld Bytes %lld\n"_Stz,iSeek,nBytes);
    
    if (iSeek) {
      m_oFile.vSeek(iSeek - 1);
      i8 nBytesRead = nReadBytes(1);
      ASSERT(1 == nBytesRead);
      ASSERT('\n' == aDst[0]);
    }
    
    m_oFile.vSeek(iSeek + nBytes - 1);
    i8 nBytesRead = nReadBytes(1);
    ASSERT(1 == nBytesRead);
    ASSERT('\n' == aDst[0]);
    
  }
}






/*----------------------------------------------------------------------------*/
/* parent adds the offset of the end of a read-chunk                          */
/*----------------------------------------------------------------------------*/
void LoadMap::vEolOffset(i8 iEolOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);

  m_zEolLock.vGrabWrite();
  
  ++gpAttachMap->m_oLockTracker.m_ziWorkUnits[LK_LOAD_MAP];
  
  m_iEolOffset[m_nEntries++] = iEolOffset;

  m_zEolLock.vDropWrite();
  
  RASSERT(m_nEntries < C_LOADMAP_ENTRIES);
}

/*----------------------------------------------------------------------------*/
/* has the parent completed this map yet?                                     */
/*----------------------------------------------------------------------------*/
bool LoadMap::bMappingDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  return m_bMappingDone;
}

/*----------------------------------------------------------------------------*/
/* the parent has completed this map                                          */
/*----------------------------------------------------------------------------*/
void LoadMap::vMappingDone(bool bMappingDone)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_bMappingDone = bMappingDone;
  
  //vTrace("LoadMap closed with %lld entries\n"_Stz,m_nEntries);
}

/*----------------------------------------------------------------------------*/
/* subordinate takes next read-chunk                                          */
/*----------------------------------------------------------------------------*/
bool LoadMap::bNextChunk(i8& iSeek, i8& nBytes, i8& i_Chunk, i8& nChunks)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);

  // is there another entry available yet?
  if (m_iNext == m_nEntries) {
    return false;
  }
  
  // subordinates compete for next entries
  m_zEolLock.vGrabWrite();
 
  // is there another entry available yet?
  if (m_iNext == m_nEntries) {
    m_zEolLock.vDropWrite();
    return false;
  }
  ASSERT(m_iNext < m_nEntries);
  
  // a subsequent subordinate calling this func will get the next entry
  // save m_iNext before incrementing
  i_Chunk  = m_iNext++;
  
  // now ok to drop the lock
  m_zEolLock.vDropWrite();
  
  // - a seekmap holds ending offsets only
  nChunks = m_nEntries;
  
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
    
    if (m_bMappingDone && ! bMoreChunks)
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
         m_zEolLock.iWriteGrabs(),
         m_zEolLock.iWriteCollisions());
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
