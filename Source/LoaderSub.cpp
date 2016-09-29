/*============================================================================*/
/* LoaderSub.cpp -- Subordinate Parallel Table Loader                         */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoaderSub* DBobjects::_pLoaderSub(void* pAddr)
{
  LoaderSub* pLoader = reinterpret_cast<LoaderSub*>(pAddr);
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* LoaderSub::operator new(size_t iSize)
{
  RASSERT(iSize);
  void* pObject = gpSmallHeap->pAllocObject(CN_LOADERSUB);
  
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
  
  gpSmallHeap->vFreeObject(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoaderSub::LoaderSub(void)
: Object(CN_LOADERSUB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);

  m_pLoader = nullptr;
  m_pRecord = nullptr;
  m_iLength = 0;
  m_iHash   = 0;
  m_iPartition  = 0;
  m_bMoreChunks = false;
  m_pLoadChunk  = new LoadChunk;
  
  for (i8 iPartition = 0; iPartition < C_TABLE_PARTITIONS; iPartition++)
    m_pRowBuffer[iPartition] = nullptr;
  
  _checkInitialised_(sizeof(LoaderSub));
  
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
void LoaderSub::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADERSUB);
  
  LoaderSub* pLoader = reinterpret_cast<LoaderSub*>(pObject);
  
  vTrace("%LoaderSub %p:\n", pTab, pLoader);
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
  
  sPath = m_pLoader->sPath();
  
  // open the chunked file
  m_pLoadChunk->vOpenFile(sPath);
  
  // read first chunk
  m_bMoreChunks = bReadChunk();
}

/*----------------------------------------------------------------------------*/
/* this sub-loader has completed                                              */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bCompleted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  bool bIsDecider = m_pLoader->bSubCompleted();
  
  return bIsDecider;
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void LoaderSub::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  TxRef* pTxRef = m_pLoader->pTxRef();
  
  // insert any dangling buffers
  for (i8 iPartition = 0; iPartition < C_TABLE_PARTITIONS; iPartition++) {
    if (m_pRowBuffer[iPartition]) {
      RowBuffer* pRowBuffer = m_pRowBuffer[iPartition];
      m_pRowBuffer[iPartition] = nullptr;
      m_pLoader->vAppendBuffer(m_iPartition,pRowBuffer);
    }
  }
  
  // close the file being loaded
  m_pLoadChunk->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read more if necessary                      */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  if ( ! m_bMoreChunks )
    return false;
  
  // fetch next record from current chunk
  m_pRecord = m_pLoadChunk->pNextRecord(m_iLength);
  if ( m_pRecord ) {
    //vDumpAscii(m_pRecord,m_iLength);
    return true;
  }
  
  // read next chunk
  m_bMoreChunks = bReadChunk();
  if (! m_bMoreChunks)
    return false;
  
  // fetch first record from chunk just read
  m_pRecord = m_pLoadChunk->pNextRecord(m_iLength);
  if (m_pRecord)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* hash the first field of record                                             */
/*----------------------------------------------------------------------------*/
void LoaderSub::vHashRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);

  // calculate length of first field
  i8 iLength = 0;
  char* pChar = m_pRecord;
  while ('|' != *pChar++)
    iLength++;
  
  // hash the field using xxhash
  m_iHash = XXH32(m_pRecord,iLength,0);
  
  // modulo to get partition number
  m_iPartition = m_iHash % gxArguments.m_nPartitions;
  
  //vTrace("Partn %lld\n",m_iPartition);
}

/*----------------------------------------------------------------------------*/
/* insert row into working buffer                                             */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  //vTrace("Row: %s\n",pStrVal->sStr());
  
  // check for working buffer
  if (! m_pRowBuffer[m_iPartition])
    m_pRowBuffer[m_iPartition] = new RowBuffer;
  
  // get working buffer
  RowBuffer* pRowBuffer = m_pRowBuffer[m_iPartition];

  TxRef* pTxRef = m_pLoader->pTxRef();

  // insert row into working buffer
  if (pRowBuffer->bInsertRow(pTxRef,m_pRecord,m_iLength))
    return true;
  
  // buffer full, append working buffer into table
  m_pLoader->vAppendBuffer(m_iPartition,pRowBuffer);
  
  // get a new working buffer
  m_pRowBuffer[m_iPartition] = new RowBuffer;
  pRowBuffer = m_pRowBuffer[m_iPartition];
  
  // insert row into new working buffer
  bool bInserted = pRowBuffer->bInsertRow(pTxRef,m_pRecord,m_iLength);
  
  return bInserted;
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
/* read next chunk from file                                                  */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bReadChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  i8 iSeek;
  i8 nBytes;

  // get next chunk info
  if (! m_pLoader->bNextChunk(iSeek,nBytes))
    return false;
  
  // read the chunk
  m_pLoadChunk->vReadChunk(iSeek,nBytes);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* LoaderSub::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/