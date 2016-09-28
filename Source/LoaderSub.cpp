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

  m_pLoader     = nullptr;
  m_pRecord     = nullptr;
  m_iLength     = 0;
  m_iHash       = 0;
  m_iPartition  = 0;
  m_bMoreChunks = false;
  m_pLoadBuffer = new LoadBuffer;
  
  for (i8 iPartition = 0; iPartition < C_TABLE_PARTITIONS; iPartition++)
    m_pRowBuffer[iPartition] = nullptr;
  
  //--
  
  //m_xFile
  
  _checkInitialised_(sizeof(LoaderSub));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoaderSub::~LoaderSub(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  if (m_pLoadBuffer)
    delete m_pLoadBuffer;
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
  
  // open the file
  m_xFile.vPath(m_pLoader->pFile()->sPath());
  bool b = m_xFile.bOpen(C_OPEN_READ);
  RASSERT(b);
  
  // read first chunk
  m_bMoreChunks = bFetchChunk();
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
  
  for (i8 iPartition = 0; iPartition < C_TABLE_PARTITIONS; iPartition++) {
    if (m_pRowBuffer[iPartition]) {
      RowBuffer* pRowBuffer = m_pRowBuffer[iPartition];
      m_pRowBuffer[iPartition] = nullptr;
      m_pLoader->vAppendBuffer(m_iPartition,pRowBuffer);
    }
  }
  
  m_xFile.vClose();  
}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read more if necessary                      */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  if ( ! m_bMoreChunks )
    return false;
  
//  vTrace("NextRecord @ %lld end %lld\n",
//         m_pLoadBuffer->iNextRecord(),m_pLoadBuffer->iEndChunk());
  
  // fetch next record from current chunk
  m_pRecord = m_pLoadBuffer->pNextRecord(m_iLength);
  if ( m_pRecord ) {
//    vDumpAscii(m_pRecord,m_iLength);
    return true;
  }
  
  // read next chunk
  m_bMoreChunks = bFetchChunk();
  if (! m_bMoreChunks )
    return false;
  
  // fetch first record from next chunk
  m_pRecord = m_pLoadBuffer->pNextRecord(m_iLength);
  if ( m_pRecord )
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* fetch next chunk from file                                                 */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bFetchChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);

  bool bMore = m_pLoader->bFetchChunk(m_pLoadBuffer);

  return bMore;
}

/*----------------------------------------------------------------------------*/
/* get record                                                                 */
/*----------------------------------------------------------------------------*/
void LoaderSub::vGetRecord(StrVal* pStrVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  Memory::vCopy(pStrVal->sStr(),m_pRecord,m_iLength);
  pStrVal->sStr()[m_iLength] = '\0';
  pStrVal->vSize();
}

/*----------------------------------------------------------------------------*/
/* hash the first field of record                                             */
/*----------------------------------------------------------------------------*/
void LoaderSub::vHashRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);

  i8 iLength = 0;
  char* pChar = m_pRecord;
  while ('|' != *pChar++)
    iLength++;
  
  m_iHash = XXH32(m_pRecord,iLength,0);
  
  m_iPartition = m_iHash % gxArguments.m_nPartitions;
  
//  vTrace("Partn %lld\n",m_iPartition);
}

/*----------------------------------------------------------------------------*/
/* insert row                                                                 */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  //vTrace("Row: %s\n",pStrVal->sStr());
  
  //bool bResult = m_pLoader->bInsertRow(m_iPartition,m_pRecord,m_iLength);
  
  if (! m_pRowBuffer[m_iPartition]) {
    m_pRowBuffer[m_iPartition] = new RowBuffer;
  }
  
  RowBuffer* pRowBuffer = m_pRowBuffer[m_iPartition];
  TxRef* pTxRef = m_pLoader->pTxRef();

  bool bInserted = pRowBuffer->bInsertRow(pTxRef,m_pRecord,m_iLength);
  
  if (bInserted)
    return true;
  
  // buffer full
  m_pLoader->vAppendBuffer(m_iPartition,pRowBuffer);
  
  m_pRowBuffer[m_iPartition] = new RowBuffer;
  pRowBuffer = m_pRowBuffer[m_iPartition];
  bInserted = pRowBuffer->bInsertRow(pTxRef,m_pRecord,m_iLength);
  
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