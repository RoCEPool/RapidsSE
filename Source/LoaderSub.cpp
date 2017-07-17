/*============================================================================*/
/* LoaderSub.cpp -- Subordinate Parallel Table Loader                         */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Grid*  gpGrid;
extern bool   gbProgress;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoaderSub* DBobjects::_pLoaderSub(void* aAddr)
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
  m_pTxRef   = nullptr;
  m_pLoadMap = nullptr;
#ifdef _ROWFORMAT
  //m_aRecord
#else
  m_aRecord  = nullptr;
#endif
  m_iLength  = 0;
  m_iPartition  = 0;
  m_bMoreChunks = false;
  m_pLoadChunk  = new LoadChunk;
  
  for (i8 i_Partition = 0; i_Partition < C_MAX_PARTITIONS; i_Partition++)
    m_pEntryBuffer[i_Partition] = nullptr;
  
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
void LoaderSub::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_LOADERSUB);
  ASSERT(pObject->eClass() == CN_LOADERSUB);
  
  LoaderSub* pLoader = reinterpret_cast<LoaderSub*>(pObject);
  
  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append working buffer into table                                           */
/*----------------------------------------------------------------------------*/
void LoaderSub::vAppendBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  // get working buffer
  EntryBuffer* pEntryBuffer = m_pEntryBuffer[m_iPartition];
  m_pEntryBuffer[m_iPartition] = nullptr;
  
  // append working buffer into table
  m_pTable->vAppendBuffer(m_iPartition,pEntryBuffer);
}

/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
void LoaderSub::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  s256 sPath;
  
  // get table, tx and load map
  m_pTable   = m_pLoader->pTable();
  m_pTxRef   = m_pLoader->pTxRef();
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
/*
    if (m_pEntryBuffer[i_Partition]) {
      EntryBuffer* pEntryBuffer = m_pEntryBuffer[i_Partition];
      m_pEntryBuffer[i_Partition] = nullptr;
      m_pTable->vAppendBuffer(m_iPartition,pEntryBuffer);
    }
*/
    ASSERT(! m_pEntryBuffer[i_Partition]);
  }
  
  // close the file being loaded
  m_pLoadChunk->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
bool LoaderSub::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  Str* aRawData;
  i8 iLength;
  
  if (! m_bMoreChunks)
    return false;
  
  // fetch next record from current chunk
  aRawData = m_pLoadChunk->aNextRecord(iLength);
  if (aRawData) {
    //vDumpAscii(m_aRecord,m_iLength);
    // format the raw data into a record
    RowData::vFormatData(aRawData, iLength, m_aRecord.aStr(), m_iLength);
    return true;
  }
  
  // read next chunk
  m_bMoreChunks = bReadChunk();
  if (! m_bMoreChunks)
    return false;
  
  // fetch first record from chunk just read
  aRawData = m_pLoadChunk->aNextRecord(iLength);
  if (aRawData) {
    // format the raw data into a record
    RowData::vFormatData(aRawData, iLength, m_aRecord.aStr(), m_iLength);
    return true;
  }
  
  return false;
}

#else
bool LoaderSub::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  if (! m_bMoreChunks)
    return false;
  
  // fetch next record from current chunk
  m_aRecord = m_pLoadChunk->aNextRecord(m_iLength);
  if (m_aRecord) {
    //vDumpAscii(m_aRecord,m_iLength);
    return true;
  }
  
  // read next chunk
  m_bMoreChunks = bReadChunk();
  if (! m_bMoreChunks)
    return false;
  
  // fetch first record from chunk just read
  m_aRecord = m_pLoadChunk->aNextRecord(m_iLength);
  if (m_aRecord) {
    return true;
  }
  
  return false;
}
#endif
/*----------------------------------------------------------------------------*/
/* hash the first field of record                                             */
/*----------------------------------------------------------------------------*/
void LoaderSub::vHashRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);

  // calculate length of first field
  //  i8 iLength = 0;
  //  char* pChar = (char*)m_aRecord;
  //  while ('|' != *pChar++)
  //    iLength++;
  
  // hash the field using xxhash
  //m_iHash = XXH32(m_aRecord,iLength,0);
  //m_iPartition = m_iHash % (gpGrid->iSlots() * gpGrid->iNodes());
  
  m_iPartition = m_pTable->iRowPartition(m_aRecord);
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
 
  //vTrace("%s\n"_Str, m_aRecord->s());
  /*
  // get the hidden index data
  Partition* pPartition = m_pTable->pPartition(m_iPartition);
  
  // insert an entry into the hidden index data
  IndexData* pIndexData = pPartition->pHiddenIndexData();
  Entry* pRowEntry = pIndexData->pInsertEntry(m_pTxRef->iTxId(), m_aRecord, m_iLength);

  // get no of indexes of the table
  IdxDataVec* pIdxDataVec = pPartition->pIdxDataVec();
  i8 nIndexes = pIdxDataVec->nItems();

  // insert an entry for the row in all the indexes of this table
  for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
    
    IndexData* pIndexData = pIdxDataVec->pGet(i_Index);
    Index* pIndex = pIndexData->pIndex();
    
    // skip the hidden index, as already inserted row in it
    if ( ! pIndex->bIsHidden())      
      pIndexData->vInsertEntry(pRowEntry);
  }
  
  return true;
  */
  //vDumpAscii(m_aRecord,m_iLength);
  
  // check for working buffer
  if (! m_pEntryBuffer[m_iPartition]) {
    m_pEntryBuffer[m_iPartition] = new EntryBuffer(m_pTable->pHiddenIndex(),0);
    if (! m_pEntryBuffer[m_iPartition]) {
      vTrace("EntryBuffer allocation failed\n"_Str);
      return false;
    }
  }
  
  // get working buffer
  EntryBuffer* pEntryBuffer = m_pEntryBuffer[m_iPartition];
  
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!truncate!!
  
//  i8 iLength = m_iLength;
//  
//  char* aCursor = (char*)m_aRecord;
//  bool bClip = StrLib::bFind(&aCursor,'|',13);
//  iLength = nSub(aCursor,m_aRecord) - 1;
//
//  char xInsBuf[256];
//  StrLib::vCopyLen(xInsBuf,(char*)m_aRecord,iLength);
//  //vTrace("Inserting: %s %lld\n"_Str,xInsBuf,iLength);
//  
//  // insert row into working buffer
//  bool b = pEntryBuffer->bInsertEntry(m_pTxRef->iTxId(),m_aRecord,iLength);
  
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  // insert row into working buffer
  bool b = pEntryBuffer->bInsertEntry(m_pTxRef->iTxId(),m_aRecord,m_iLength);
  
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  return b;
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
    
    //vTrace("Waiting for map\n"_Str);
  }
  
  if (gbProgress) {
    if (0 == iChunk % 100) {
      vInfo("read chunk %lld of %lld        \r"_Str,iChunk,nChunks);
      fflush(stdout);
    }
  }
  
  // read the chunk
  m_pLoadChunk->vReadChunk(iSeek,nBytes);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get rowBuffer                                                              */
/*----------------------------------------------------------------------------*/
EntryBuffer* LoaderSub::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = m_pEntryBuffer[m_iPartition];

  //m_pRowBuffer[m_iPartition] = nullptr;
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* set rowBuffer                                                              */
/*----------------------------------------------------------------------------*/
void LoaderSub::vRowBuffer(EntryBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  m_pEntryBuffer[m_iPartition] = pRowBuffer;
  
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
/* validate row                                                               */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
bool LoaderSub::bValidateRow(i8& i_Column)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  bool bValid = m_pTable->bValidateRow(m_aRecord,i_Column);
  
  if ( ! bValid ) {
    vTrace("Loader:: Invalid field %lld in record: "_Str,
           i_Column);
    for (char* a = (char*)m_aRecord.s(); *a && '\n' != *a; a++)
      vTrace("%c"_Str,*a);
    vTrace("\n"_Str);
    exit(-1);
  }
  
  return bValid;
}

#else
bool LoaderSub::bValidateRow(i8& i_Column)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);

  bool bValid = m_pTable->bValidateRow(m_aRecord,i_Column);
  
  if ( ! bValid ) {
    vTrace("Loader:: Invalid field %lld in record: "_Str,
           i_Column);
    for (char* a = (char*)m_aRecord; *a && '\n' != *a; a++)
      vTrace("%c"_Str,*a);
    vTrace("\n"_Str);
    exit(-1);
  }
  
  return bValid;
}
#endif
/*----------------------------------------------------------------------------*/
/* truncate record from given field                                           */
/*----------------------------------------------------------------------------*/
//void LoaderSub::vTruncateRecord(i8 iField)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
//  
//  char* aCursor = (char*)m_aRecord;
//  
//  vTrace("Before %s\n"_Str,m_aRecord);
//  
//  bool b = StrLib::bFind(&aCursor,'|',iField);
//  
//  if (b) {
//    *aCursor = '\0';
//    m_iLength = StrLib::iLength((char*)m_aRecord);
//
//    vTrace("After %s\n"_Str,m_aRecord);
//  }
//}

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
