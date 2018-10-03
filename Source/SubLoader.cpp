/*============================================================================*/
/* SubLoader.cpp -- Subordinate Parallel Table LoadTable                      */
/*============================================================================*/

#include "Utilities.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SubLoader* Utilities::_pLoaderSub(void* aAddr)
{
  SubLoader* pSubLoader = reinterpret_cast<SubLoader*>(aAddr);
  
  RASSERT( !pSubLoader || CN_SUBLOADER == pSubLoader->eClass() );
  
  return pSubLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* SubLoader::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = HeapMem::pAllocate(CN_SUBLOADER);
 
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* SubLoader::operator new(size_t iSize, SubLoader* pSubLoader)
{
  RASSERT(iSize);
  
  return pSubLoader;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void SubLoader::operator delete(void* pLoaderAddress)
{
  SubLoader* pLoadTable = reinterpret_cast<SubLoader*>(pLoaderAddress);
  
  HeapMem::vFree(pLoadTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubLoader::SubLoader(Job_SubLoader* pJob_SubLoader, LoadTable* pLoadTable)
: Object(CN_SUBLOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);

  m_pJob_SubLoader = pJob_SubLoader;
  m_pLoadTable  = pLoadTable;
  m_pLoadChunk  = new LoadChunk;

  m_pTable   = m_pJob_SubLoader->m_pJob_LoadTable->m_pTable;
  m_pLoadMap = nullptr;
  //m_aRecord
  m_iLength  = 0;
  m_iPartition  = 0;
  m_bMoreChunks = false;
  m_iValidRecords = 0;
  m_iInvalidRecords = 0;
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
    m_pWorkingBuffer[i_Partition]   = nullptr;
    m_nBuffersLoadable[i_Partition] = 0;
  }
  
  _checkInitialised_(CN_SUBLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SubLoader::~SubLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  if (m_pLoadChunk)
    delete m_pLoadChunk;
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void SubLoader::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SUBLOADER);
  RASSERT(pObject->eClass() == CN_SUBLOADER);
  
  SubLoader* pLoadTable = reinterpret_cast<SubLoader*>(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
void SubLoader::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  s256 sPath;
  sPath = m_pLoadTable->pJob_LoadTable()->m_sFileName;
  
  // open the chunked file
  m_pLoadChunk->vOpenFile(sPath);
  
  // read first chunk
  m_bMoreChunks = bReadChunk();
}

/*----------------------------------------------------------------------------*/
/* get buffers loadable for implicit partition                                */
/*----------------------------------------------------------------------------*/
i8 SubLoader::nBuffersLoadable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  return m_nBuffersLoadable[m_iPartition];
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void SubLoader::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  //for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++) {
  //  ASSERT(! m_pWorkingBuffer[i_Partition]);
  //}
  
  // close the file being loaded
  m_pLoadChunk->vCloseFile();

}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
bool SubLoader::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  Stz* aRawData;
  i8 iLength;
  bool bValid = false;
  bool bBlankLine;
  
  Job_LoadTable* pJob_LoadTable = m_pLoadTable->pJob_LoadTable();
  Formatters* pFormatters = &pJob_LoadTable->m_xFormatters;
  Table* pTable = pJob_LoadTable->m_pTable;
  
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
    
    if (iLength > C_16KB) {
      m_iInvalidRecords++;
      vWarn("Skipping row %.20s... - record in file > 16KB\n"_Stz,
            aRawData);
      return false;
    }

    // format the raw data into a row
    bValid = Row::bFormatRow(pTable->pColumnVec(),
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
/* hash the first field of record                                             */
/*----------------------------------------------------------------------------*/
void SubLoader::vHashRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  Table* pTable = m_pJob_SubLoader->m_pJob_LoadTable->m_pTable;
  
  m_iPartition = pTable->iRowPartition(m_aRecord);
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
bool SubLoader::bInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
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
  //SEitf* pSEitf = m_pLoadTable->pSEitf();
  //Tx* pTx = pSEitf->pTx();
  //i8 iDistTxId = pTx->iDistTxId();
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
i8 SubLoader::iInvalidRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  return m_iInvalidRecords;
}

/*----------------------------------------------------------------------------*/
/* get loader                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubLoader* SubLoader::pJob_SubLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  return m_pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* process records                                                            */
/*----------------------------------------------------------------------------*/
bool SubLoader::bProcessRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  while (bFetchRecord()) {
    
    // hash the record to get m_iPartition
    vHashRecord();
    
    // attempt to add record into a buffer
    bool bInserted = bInsertRow();
    
    // check if target buffer was full
    if (! bInserted) {
      // buffer was full
      // return to loaderSub to append buffer to the table/partition
      return true;
    }
    
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* create a new working buffer                                                */
/*----------------------------------------------------------------------------*/
void SubLoader::vNewWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  m_pWorkingBuffer[m_iPartition] = new WorkingBuffer(m_pTable, m_iPartition);
  m_nBuffersLoadable[m_iPartition]++;
}

/*----------------------------------------------------------------------------*/
/* get partition number                                                       */
/*----------------------------------------------------------------------------*/
i8 SubLoader::iPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* set partition number                                                       */
/*----------------------------------------------------------------------------*/
void SubLoader::vPartition(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  m_iPartition = iPartition;
}

/*----------------------------------------------------------------------------*/
/* read next chunk from file                                                  */
/*----------------------------------------------------------------------------*/
bool SubLoader::bReadChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  i8 iSeek;
  i8 nBytes;
  i8 iChunk;
  i8 nChunks;
  
  LoadMap* pLoadMap = m_pLoadTable->pLoadMap();


  // get information about next chunk
  for (;;) {
    
    bool bMapLoaded = pLoadMap->bDone();
    
    bool bMoreChunks = pLoadMap->bNextChunk(iSeek,nBytes,iChunk,nChunks);
    
    if (bMapLoaded && ! bMoreChunks)
      return false;
    
    if (bMoreChunks)
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
/* get workingBuffer                                                          */
/*----------------------------------------------------------------------------*/
WorkingBuffer* SubLoader::pWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  WorkingBuffer* pWorkingBuffer = m_pWorkingBuffer[m_iPartition];

  //m_pRowBuffer[m_iPartition] = nullptr;
  
  return pWorkingBuffer;
}

/*----------------------------------------------------------------------------*/
/* set workingBuffer                                                          */
/*----------------------------------------------------------------------------*/
void SubLoader::vWorkingBuffer(WorkingBuffer* pWorkingBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  m_pWorkingBuffer[m_iPartition] = pWorkingBuffer;
  
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* SubLoader::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* get no of valid records fetched                                            */
/*----------------------------------------------------------------------------*/
i8 SubLoader::iValidRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  return m_iValidRecords;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SubLoader::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
