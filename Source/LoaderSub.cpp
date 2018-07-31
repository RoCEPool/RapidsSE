/*============================================================================*/
/* LoaderSub.cpp -- Subordinate Parallel Table Loader                         */
/*============================================================================*/

#include "Utilities.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoaderSub* Utilities::_pLoaderSub(void* aAddr)
{
  LoaderSub* pLoaderSub = reinterpret_cast<LoaderSub*>(aAddr);
  
  RASSERT( !pLoaderSub || CN_LOADERSUB == pLoaderSub->eClass() );
  
  return pLoaderSub;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* LoaderSub::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_LOADERSUB);
 
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* LoaderSub::operator new(size_t iSize, LoaderSub* pLoaderSub)
{
  RASSERT(iSize);
  
  return pLoaderSub;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void LoaderSub::operator delete(void* pLoaderAddress)
{
  LoaderSub* pLoader = reinterpret_cast<LoaderSub*>(pLoaderAddress);
  
  Heap::vFree(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoaderSub::LoaderSub(void)
: Object(CN_LOADERSUB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);

  m_pLoader  = nullptr;
  m_pTable   = nullptr;
  m_pLoadMap = nullptr;
  //m_aRecord
  m_iLength  = 0;
  m_iPartition  = 0;
  m_bMoreChunks = false;
  m_pLoadChunk  = new LoadChunk;
  m_iValidRecords = 0;
  m_iInvalidRecords = 0;
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++)
    m_pWorkingBuffer[i_Partition] = nullptr;
  
  _checkInitialised_(CN_LOADERSUB);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoaderSub::~LoaderSub(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  if (m_pLoadChunk)
    delete m_pLoadChunk;
}

/*----------------------------------------------------------------------------*/
/* dump LoaderSub                                                             */
/*----------------------------------------------------------------------------*/
void LoaderSub::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_LOADERSUB);
  RASSERT(pObject->eClass() == CN_LOADERSUB);
  
  LoaderSub* pLoader = reinterpret_cast<LoaderSub*>(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
void LoaderSub::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  s256 sPath;
  
  // get table, tx and load map
  m_pTable   = m_pLoader->pTable();
  m_pLoadMap = m_pLoader->pLoadMap();
  
  sPath = m_pLoader->sPath();
  
  // open the chunked file
  m_pLoadChunk->vOpenFile(sPath);
  
  // read first chunk
  m_bMoreChunks = bReadChunk();
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void LoaderSub::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  // append any dangling buffers
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    ASSERT(! m_pWorkingBuffer[i_Partition]);
  }
  
  // close the file being loaded
  m_pLoadChunk->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  Stz* aRawData;
  i8 iLength;
  bool bValid = false;
  bool bBlankLine;
  
  Interface::SEitf* pSEitf = m_pLoader->pSEitf();
  Formatters* pFormatters = &pSEitf->m_xFormatters;
  
  if (! m_bMoreChunks)
    return false;
  
  while (true) {
    
    // fetch next record from current chunk
    aRawData = m_pLoadChunk->aNextRecord(iLength,pFormatters);
    
    if (! aRawData) {
      // read next chunk
      m_bMoreChunks = bReadChunk();
      if (! m_bMoreChunks)
        return false;
      continue;
    }
    
    // format the raw data into a row
    bValid = Row::bFormatRow(m_pTable->pColumnVec(),
                             pFormatters,
                             aRawData, iLength,
                             m_aRecord.aStr(), m_iLength,
                             bBlankLine,
                             false /* allowNonPrintable */);
    if (bValid) {
      m_iValidRecords++;
      return bValid;
    }
    
    if ( ! bBlankLine )
      m_iInvalidRecords++;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bFetchRecordDist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  Stz* aRawData;
  i8 iLength;
  bool bValid = false;
  bool bBlankLine;
  
  if (! m_bMoreChunks)
    return false;
  
  while (true) {
    
    // fetch next record from current chunk
    aRawData = m_pLoadChunk->aNextRecord(iLength,&m_xFormatters);
    
    if (! aRawData) {
      // read next chunk
      m_bMoreChunks = bReadChunk();
      if (! m_bMoreChunks)
        return false;
      continue;
    }
    
    // format the raw data into a row
    bValid = Row::bFormatRow(m_pTable->pColumnVec(),
                             &m_xFormatters,
                             aRawData, iLength,
                             m_aRecord.aStr(), m_iLength,
                             bBlankLine,
                             false /* allowNonPrintable */);
    if (bValid) {
      m_iValidRecords++;
      return bValid;
    }
    
    if ( ! bBlankLine )
      m_iInvalidRecords++;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* hash the first field of record                                             */
/*----------------------------------------------------------------------------*/
void LoaderSub::vHashRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  m_iPartition = m_pTable->iRowPartition(m_aRecord);
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  // check for working buffer
  if (! m_pWorkingBuffer[m_iPartition]) {
    
    // create a new working buffer for this partition
    vNewWorkingBuffer();
    
    if (! m_pWorkingBuffer[m_iPartition]) {
      vTrace("EntryBuffer allocation failed\n"_Stz);
      return false;
    }
  }
  
  // get working buffer
  WorkingBuffer* pWorkingBuffer = m_pWorkingBuffer[m_iPartition];
  
  // get the tx id
  SEitf* pSEitf = m_pLoader->pSEitf();
  Tx* pTx = pSEitf->pTx();
  i8 iDistTxId = pTx->iDistTxId();
  
  RowWrk xRowWrk;
  
  xRowWrk.vTxId(iDistTxId);
  xRowWrk.vEmbedded(m_aRecord.aChar());
  xRowWrk.vRowSize(m_iLength);
  
  // insert row into working buffer
  bool bAppended = pWorkingBuffer->bAppendRow(xRowWrk);
  
  //pEntryBuffer->vShow();
  
  return bAppended;
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bInsertRowDist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  // check for working buffer
  if (! m_pWorkingBuffer[m_iPartition]) {
    
    // create a new working buffer for this partition
    vNewWorkingBuffer();
    
    if (! m_pWorkingBuffer[m_iPartition]) {
      vTrace("EntryBuffer allocation failed\n"_Stz);
      return false;
    }
  }
  
  // get working buffer
  WorkingBuffer* pWorkingBuffer = m_pWorkingBuffer[m_iPartition];
  
  // get the tx id
  i8 iDistTxId = 0;
  
  RowWrk xRowWrk;
  
  xRowWrk.vTxId(iDistTxId);
  xRowWrk.vEmbedded(m_aRecord.aChar());
  xRowWrk.vRowSize(m_iLength);
  
  // insert row into working buffer
  bool bAppended = pWorkingBuffer->bAppendRow(xRowWrk);
  
  //pEntryBuffer->vShow();
  
  return bAppended;
}

/*----------------------------------------------------------------------------*/
/* get no of invalid records fetched                                          */
/*----------------------------------------------------------------------------*/
i8 LoaderSub::iInvalidRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  return m_iInvalidRecords;
}

/*----------------------------------------------------------------------------*/
/* get loader                                                                 */
/*----------------------------------------------------------------------------*/
Loader* LoaderSub::pLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  return m_pLoader;
}

/*----------------------------------------------------------------------------*/
/* set loader                                                                 */
/*----------------------------------------------------------------------------*/
void LoaderSub::vLoader(Loader* pLoader)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);

  m_pLoader = pLoader;
}

/*----------------------------------------------------------------------------*/
/* create a new working buffer                                                */
/*----------------------------------------------------------------------------*/
void LoaderSub::vNewWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  m_pWorkingBuffer[m_iPartition] = new WorkingBuffer(m_pTable, m_iPartition);
}

/*----------------------------------------------------------------------------*/
/* get partition number                                                       */
/*----------------------------------------------------------------------------*/
i8 LoaderSub::iPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* set partition number                                                       */
/*----------------------------------------------------------------------------*/
void LoaderSub::vPartition(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  m_iPartition = iPartition;
}

/*----------------------------------------------------------------------------*/
/* read next chunk from file                                                  */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bReadChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  i8 iSeek;
  i8 nBytes;
  i8 iChunk;
  i8 nChunks;

  // get information about next chunk
  for (;;) {
    
    bool bDone = m_pLoadMap->bDone();
    
    bool bNext = m_pLoadMap->bNextChunk(iSeek,nBytes,iChunk,nChunks);
    
    if (bDone && ! bNext)
      return false;
    
    if (bNext)
      break;
    
    //vTrace("Waiting for map\n"_Stz);
  }
  
  if (gpAttachMap->m_bProgress) {
    if (0 == iChunk % 100) {
      vInfo("read chunk %lld of %lld        \r"_Stz,iChunk,nChunks);
      fflush(stdout);
    }
  }
  
  // read the chunk
  m_pLoadChunk->vReadChunk(iSeek,nBytes);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set formatter                                                              */
/*----------------------------------------------------------------------------*/
void LoaderSub::vSetDistFormatter(char cFieldDelimiter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  m_xFormatters.m_cFieldDelimiter = cFieldDelimiter;
}

/*----------------------------------------------------------------------------*/
/* get workingBuffer                                                          */
/*----------------------------------------------------------------------------*/
WorkingBuffer* LoaderSub::pWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  WorkingBuffer* pWorkingBuffer = m_pWorkingBuffer[m_iPartition];

  //m_pRowBuffer[m_iPartition] = nullptr;
  
  return pWorkingBuffer;
}

/*----------------------------------------------------------------------------*/
/* set workingBuffer                                                          */
/*----------------------------------------------------------------------------*/
void LoaderSub::vWorkingBuffer(WorkingBuffer* pWorkingBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  m_pWorkingBuffer[m_iPartition] = pWorkingBuffer;
  
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* LoaderSub::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* get no of valid records fetched                                            */
/*----------------------------------------------------------------------------*/
i8 LoaderSub::iValidRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  return m_iValidRecords;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* LoaderSub::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
