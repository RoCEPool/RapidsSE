/*============================================================================*/
/* Loader.cpp -- Parallel Table Loader                                        */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;
extern Heap*     gpMediumHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Loader* DBobjects::_pLoader(void* pAddr)
{
  Loader* pLoader = reinterpret_cast<Loader*>(pAddr);
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t iSize)
{
  RASSERT(iSize);
  void* pObject = gpMediumHeap->pAllocObject(CN_LOADER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t iSize, Loader* pLoader)
{
  RASSERT(iSize);
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Loader::operator delete(void* pLoaderAddress)
{
  Loader* pLoader = reinterpret_cast<Loader*>(pLoaderAddress);
  
  gpMediumHeap->vFreeObject(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader::Loader(void)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_pTable   = nullptr;
  //m_xFile
  m_iSeek    = 0;
  m_pTxRef   = nullptr;
  //m_xLock
  m_nSubs    = 0;
  
  m_pChunkMap = new SeekMap;
  //m_xChunkFile
  m_iChunkNext = 0;
  Memory::vZero(m_xBlockTest,C_BLOCKTEST);
  m_iChunkTell = 0;
  
  _checkInitialised_(sizeof(Loader));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pChunkMap;
}

/*----------------------------------------------------------------------------*/
/* dump Loader                                                                */
/*----------------------------------------------------------------------------*/
void Loader::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADER);
  
  Loader* pLoader = reinterpret_cast<Loader*>(pObject);
  
  vTrace("%Loader %p:\n", pTab, pLoader);
  vTrace("%s  table %s \n", pTab, pLoader->m_pTable->pTableName());
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append row buffer                                                          */
/*----------------------------------------------------------------------------*/
void Loader::vAppendBuffer(i8 iPartition, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  ASSERT(iPartition < gxArguments.m_nPartitions);
  
  m_pTable->vAppendBuffer(iPartition,pRowBuffer);
  
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void Loader::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* get file                                                                   */
/*----------------------------------------------------------------------------*/
File* Loader::pFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return &m_xFile;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool Loader::bOpenFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  bool b = m_xFile.bOpen(C_OPEN_READ);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void Loader::vPath(s& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  const char* pPath = sPath.a();
  
  m_xFile.vPath(pPath);
}

/*----------------------------------------------------------------------------*/
/* set number of subordinates                                                 */
/*----------------------------------------------------------------------------*/
void Loader::vSubs(i8 nSubs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_nSubs = nSubs;
}

/*----------------------------------------------------------------------------*/
/* subordinate completes, returns true if last one to complete                */
/*----------------------------------------------------------------------------*/
bool Loader::bSubCompleted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  bool bIsDecider = false;
  
  m_xLock.vGrabWrite();
  
  if (0 == --m_nSubs)
    bIsDecider = true;

  m_xLock.vDropWrite();
  
  return bIsDecider;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Loader::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Loader::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* set txref                                                                  */
/*----------------------------------------------------------------------------*/
TxRef* Loader::pTxRef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_pTxRef;
}

/*----------------------------------------------------------------------------*/
/* set txref                                                                  */
/*----------------------------------------------------------------------------*/
void Loader::vTxRef(TxRef* pTxRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTxRef = pTxRef;
  
  vTrace("TxId %lld\n",pTxRef->iTxId());
  
}

/*----------------------------------------------------------------------------*/
/*-- Chunking ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fetch next chunk from file                                                 */
/*----------------------------------------------------------------------------*/
bool Loader::bFindChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  if (! m_xChunkFile.bIsOpen()) {
    m_xChunkFile.vPath(m_xFile.sPath());
    bool b = m_xChunkFile.bOpen(C_OPEN_READ);
    if (! b)
      return false;
    
    m_xChunkFile.vSeekEnd();
    m_iChunkTell = m_xChunkFile.iTell();
  }
  
  if (m_iChunkNext >= m_iChunkTell) {
    m_pChunkMap->vDone(true);
    //vChunkTest();
    m_xChunkFile.vClose();
    return false;
  }
  
  i8 iTestSeek = m_iChunkNext + C_LOADBUFFER_SIZE - C_BLOCKTEST;
  
  if (iTestSeek > m_iChunkTell) {
    iTestSeek = m_iChunkTell - 1;
  }
  
  m_xChunkFile.vSeek(iTestSeek);
  
  i8 iBytesRead = m_xChunkFile.iReadChunk(m_xBlockTest,C_BLOCKTEST);
  
  const char *z = &m_xBlockTest[iBytesRead - 1];
  
  bool bFoundEol = String::bBackward(&z,m_xBlockTest,'\n');
  RASSERT(bFoundEol);
  
  i8 nBytes = Magic::iSub((void*)z,m_xBlockTest) + 1;
  
  i8 iEolOffset = iTestSeek + nBytes - 1;
  //vTrace("EolOffset %lld\n",iEolOffset);
  m_pChunkMap->vEolOffset(iEolOffset);
  
  m_iChunkNext = iEolOffset + 1;
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* test if we chunked correctly                                               */
/*----------------------------------------------------------------------------*/
void Loader::vChunkTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  i8 iSeek;
  i8 nBytes;
  
  while (m_pChunkMap->bNextChunk(iSeek,nBytes)) {
    
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
/*-- Called by LoaderSub -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fetch next chunk from file                                                 */
/*----------------------------------------------------------------------------*/
bool Loader::bFetchChunk(LoadBuffer* pLoadBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  i8 iBytesWorkarea = pLoadBuffer->iBytesWorkarea();
  char* pWorkaread  = pLoadBuffer->pWorkarea();
  
  m_xLock.vGrabWrite();
  
  m_xFile.vSeek(m_iSeek);
  
  i8 iBytesRead = m_xFile.iReadChunk(pWorkaread,iBytesWorkarea);
  //  vTrace("Read %lld\n",iBytesRead);
  
  if ( ! iBytesRead ) {
    m_xLock.vDropWrite();
    return false;
  }
  
  i8 iCompleteBytes = pLoadBuffer->iCompleteBytes(iBytesRead);
  //  vTrace("Complete %lld; NextRecord @ %lld end %lld\n",
  //         iCompleteBytes,
  //         m_pLoadBuffer->iNextRecord(),
  //         m_pLoadBuffer->iEndChunk());
  
  m_iSeek += iCompleteBytes;
  
  m_xLock.vDropWrite();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert row                                                                 */
/*----------------------------------------------------------------------------*/
bool Loader::bInsertRow(i8 iPartn, char* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  ASSERT(iPartn < gxArguments.m_nPartitions);
  
  m_pTable->vInsertRow(iPartn,m_pTxRef,pRecord,iLength);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* subordinate takes next read-chunk                                          */
/*----------------------------------------------------------------------------*/
bool Loader::bNextChunk(i8& iSeek, i8& nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKMAP);
  
  bool b = m_pChunkMap->bNextChunk(iSeek,nBytes);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Loader::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/