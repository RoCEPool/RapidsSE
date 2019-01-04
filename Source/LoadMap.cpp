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
: FileChunk(CN_LOADMAP,&gpAttachMap->m_xFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_sFilePath = aFilePath;
  m_i_EofLoc = 0;
  m_i_ChunkLoc = 0;
  m_bMappingDone = false;

  m_zMapLock.vInitLock(LK_LOAD_MAP);
  
  for (i8 i_Entry = 0; i_Entry < C_LOADMAP_ENTRIES; i_Entry++)
    m_xMap.iaEndLoc[i_Entry] = -1;
  m_xMap.nEntries = 0;
  m_xMap.i_Entry = 0;

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
  for (i8 i_Entry = 0; i_Entry < m_xMap.nEntries; i_Entry++) {
    if (i_Entry && 0 == i_Entry % 8)
      vTrace("\n"_Stz);
    vTrace("%ld "_Stz,m_xMap.iaEndLoc[i_Entry]);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* map next chunk in file                                                     */
/*----------------------------------------------------------------------------*/
bool LoadMap::bMapChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  // are we at the end of the file?
  if (m_i_ChunkLoc >= m_i_EofLoc) {
    vMappingDone(true);
    return false;
  }
  
  // compute the location of the final block of the current chunk
  i8 i_FinalBlockLoc = m_i_ChunkLoc + C_LOAD_CHUNK - C_LOADMAP_BLOCK;
  
  // the final block will be past the eof if the chunk is
  // less than maximum chunk size
  if (i_FinalBlockLoc > m_i_EofLoc) {
    vAddEntry(m_i_EofLoc - 1);
    vMappingDone(true);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process a chunk
  
  // read final block of the chunk
  char* aFinalBlock = aDataArea();
  m_oFile.vSeek(i_FinalBlockLoc);
  i8 nBytesRead = m_oFile.nReadBytes(aFinalBlock,C_LOADMAP_BLOCK);
  
  // compute last actual byte of final block just read in
  const char *aLastByte = &aFinalBlock[nBytesRead - 1];

  // find the final end of line for the block (and therefore the chunk)
  const char *aFinalEol = aLastByte;
  bool bFoundEol = StrLib::bBackward(&aFinalEol,aFinalBlock,'\n');
  if (!bFoundEol)
    bFoundEol = StrLib::bBackward(&aFinalEol,aFinalBlock,'\r');
  RASSERT(bFoundEol);
  
  // compute the number of bytes which belong to the current chunk
  i8 nPartialBytes = Base::nSub((void*)aFinalEol,aFinalBlock) + 1;
  
  // compute end location of chunk
  i8 i_EndChunkLoc = i_FinalBlockLoc + nPartialBytes - 1;
  vAddEntry(i_EndChunkLoc);
  
  // advance to start of next chunk
  m_i_ChunkLoc = i_EndChunkLoc + 1;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* has the mapping of this file completed yet?                                */
/*----------------------------------------------------------------------------*/
bool LoadMap::bMappingDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  return m_bMappingDone;
}

/*----------------------------------------------------------------------------*/
/* set mapping completion                                                     */
/*----------------------------------------------------------------------------*/
void LoadMap::vMappingDone(bool bMappingDone)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_bMappingDone = bMappingDone;
  
  //vTrace("Mapping done with %lld entries\n"_Stz,m_xMap.nEntries);
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
  
  // get the end-of-file location
  m_oFile.vSeekEnd();
  m_i_EofLoc = m_oFile.iTell();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get information about next chunk from map                                  */
/*----------------------------------------------------------------------------*/
bool LoadMap::bNextChunk(i8& i_Seek, i8& nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);

  i8 i_Entry;
  
  // mapping may be in progress, so loop till we can get next mapping
  for (;;) {
    
    // attempt to get next mapping
    bool bGotMapping = bGetNextMapping(i_Seek,nBytes,i_Entry);
    
    // did we get a mapping?
    if (bGotMapping)
      break;
    
    // if mapping is done then no more chunks to process
    if (m_bMappingDone)
      return false;
    
    //vTrace("Waiting for map\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // got a mapping
  
  // print progress
  if (gpAttachMap->m_bProgress) {
    if (0 == i_Entry % 100) {
      vInfo("read chunk %lld of %lld        \r"_Stz,i_Entry,m_xMap.nEntries);
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
         m_zMapLock.iWriteGrabs(),
         m_zMapLock.iWriteCollisions());
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add entry to map                                                           */
/*----------------------------------------------------------------------------*/
void LoadMap::vAddEntry(i8 i_ChunkEndLoc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  m_zMapLock.vGrabWrite();
  
  ++gpAttachMap->m_oLockTracker.m_ziWorkUnits[LK_LOAD_MAP];
  
  m_xMap.iaEndLoc[m_xMap.nEntries] = i_ChunkEndLoc;
  
  m_xMap.nEntries++;
  
  m_zMapLock.vDropWrite();
  
  RASSERT(m_xMap.nEntries < C_LOADMAP_ENTRIES);
}

/*----------------------------------------------------------------------------*/
/* subordinate takes next read-chunk                                          */
/*----------------------------------------------------------------------------*/
bool LoadMap::bGetNextMapping(i8& iSeek, i8& nBytes, i8& i_Entry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP);
  
  // subLoaders compete for next entries
  m_zMapLock.vGrabWrite();
  
  // is there another entry available yet?
  if (m_xMap.i_Entry == m_xMap.nEntries) {
    m_zMapLock.vDropWrite();
    return false;
  }
  ASSERT(m_xMap.i_Entry < m_xMap.nEntries);
  
  // which chunk is it
  i_Entry = m_xMap.i_Entry++;
  
  // ok to drop the lock
  m_zMapLock.vDropWrite();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // calculate starting seek offset of next read-chunk
  if (0 == i_Entry)
    iSeek = 0;
  else
    iSeek = m_xMap.iaEndLoc[i_Entry - 1] + 1;
  
  // calculate the number of bytes in the read-chunk
  i8 iEnd = m_xMap.iaEndLoc[i_Entry];
  nBytes  = iEnd - iSeek + 1;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if we chunked correctly                                               */
/*----------------------------------------------------------------------------*/
void LoadMap::vTestMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADMAP,_FOOTER_);
  
  i8 i_Seek;
  i8 nBytes;
  i8 i_Entry;
  
  char* aDst = aDataArea();
  
  while (bGetNextMapping(i_Seek,nBytes,i_Entry)) {
    
    if (gpAttachMap->m_bProgress) {
      if (0 == i_Entry % 1000) {
        vInfo("chunk %lld (of %lld) @ %lld     \r"_Stz,
              i_Entry,m_xMap.nEntries,i_Seek);
        fflush(stdout);
      }
    }
    
    //vTrace("Test Seek %lld Bytes %lld\n"_Stz,iSeek,nBytes);
    
    if (i_Seek) {
      m_oFile.vSeek(i_Seek - 1);
      i8 nBytesRead = nReadBytes(1);
      ASSERT(1 == nBytesRead);
      ASSERT('\n' == aDst[0]);
    }
    
    m_oFile.vSeek(i_Seek + nBytes - 1);
    i8 nBytesRead = nReadBytes(1);
    ASSERT(1 == nBytesRead);
    ASSERT('\n' == aDst[0]);
  }
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
