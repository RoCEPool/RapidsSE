/*============================================================================*/
/* Loader2.cpp -- Table Loader                                                */
/*============================================================================*/

#define USE_LOADBUFFER

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Loader2* DBobjects::_pLoader2(void* pAddr)
{
  Loader2* pLoader = reinterpret_cast<Loader2*>(pAddr);
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader2::operator new(size_t iSize)
{
  ASSERT(iSize);
  void* pObject = gpSmallHeap->pAllocObject(CN_LOADER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader2::operator new(size_t iSize, Loader2* pLoader)
{
  ASSERT(iSize);
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Loader2::operator delete(void* pLoaderAddress)
{
  Loader2* pLoader = reinterpret_cast<Loader2*>(pLoaderAddress);
  
  gpSmallHeap->vFreeObject(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader2::Loader2(void)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_pTable      = nullptr;
  //m_xFile
  m_iSeek       = 0;
  m_pRecord     = nullptr;
  m_pLoadBuffer = nullptr;
  m_iLength     = 0;
  m_pTxRef      = nullptr;
  
  _checkInitialised_(sizeof(Loader2));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader2::~Loader2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  if (m_pLoadBuffer)
    delete m_pLoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Loader2::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Loader2::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void Loader2::vPath(s& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  const char* pPath = sPath.a();
  
  m_xFile.vPath(pPath);
}

/*----------------------------------------------------------------------------*/
/* set txref                                                                  */
/*----------------------------------------------------------------------------*/
void Loader2::vTxRef(TxRef* pTxRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTxRef = pTxRef;
  
  vTrace("TxId %lld\n",pTxRef->iTxId());
  
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool Loader2::bOpenFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  bool b = m_xFile.bOpen(C_OPEN_READ);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
void Loader2::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  ASSERT( ! m_pLoadBuffer );
  
  m_pLoadBuffer = new LoadBuffer;
  
#ifdef USE_LOADBUFFER
  
  // read first chunk
  bFetchChunk();
  
#endif
  
}

/*----------------------------------------------------------------------------*/
/* dump Loader2                                                                */
/*----------------------------------------------------------------------------*/
void Loader2::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADER);
  
  Loader2* pLoader = reinterpret_cast<Loader2*>(pObject);
  
  vTrace("%Loader2 %p:\n", pTab, pLoader);
  vTrace("%s  table %s \n", pTab, pLoader->m_pTable->pTableName());
  vTrace("%s  record %p\n\n", pTab, pLoader->m_pRecord);
  vTrace("%s  record length %lld\n\n", pTab, pLoader->m_iLength);
}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read more if necessary                      */
/*----------------------------------------------------------------------------*/
bool Loader2::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
#ifdef USE_LOADBUFFER
  
  // fetch next record from current chunk
//  vTrace("NextRecord @ %lld end %lld\n",
//         m_pLoadBuffer->iNextRecord(),m_pLoadBuffer->iEndChunk());
  
  m_pRecord = m_pLoadBuffer->pNextRecord(m_iLength);
  if ( m_pRecord ) {
//    vDumpAscii(m_pRecord,m_iLength);
    return true;
  }
  
  // read next chunk
  bool bMore = bFetchChunk();
  if (!bMore)
    return false;
  
  // fetch first record from next chunk
  m_pRecord = m_pLoadBuffer->pNextRecord(m_iLength);
  if ( m_pRecord )
    return true;
  
  return false;
  
#else

  // fetch next record from current chunk
  m_pRecord = m_pScanBuffer->pScan(m_iLength);
  if ( m_pRecord )
    return true;
  
  // read next chunk
  bool bMore = bFetchChunk();
  if (!bMore)
    return false;
  
  // fetch first record from next chunk
  m_pRecord = m_pScanBuffer->pScan(m_iLength);
  if ( m_pRecord )
    return true;
  
  return false;
  
#endif  
}

/*----------------------------------------------------------------------------*/
/* fetch next chunk from file                                                 */
/*----------------------------------------------------------------------------*/
bool Loader2::bFetchChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
#ifdef USE_LOADBUFFER
  
  i8 iBytesWorkarea = m_pLoadBuffer->iBytesWorkarea();
  char* pWorkaread  = m_pLoadBuffer->pWorkarea();
  
  m_xFile.vSeek(m_iSeek);
  
  i8 iBytesRead = m_xFile.iReadChunk(pWorkaread,iBytesWorkarea);
//  vTrace("Read %lld\n",iBytesRead);
  
  if ( ! iBytesRead )
    return false;

  i8 iCompleteBytes = m_pLoadBuffer->iCompleteBytes(iBytesRead);
//  vTrace("Complete %lld; NextRecord @ %lld end %lld\n",
//         iCompleteBytes,
//         m_pLoadBuffer->iNextRecord(),
//         m_pLoadBuffer->iEndChunk());

  m_iSeek += iCompleteBytes;
  
  return true;

#else
  
  i8 iAvailableBytes;
  char* pInto = m_pScanBuffer->pInto(iAvailableBytes);
  
  i8 iRead = m_xFile.iReadChunk(pInto,iAvailableBytes);
  //vTrace("Read %lld\n",iRead);
  m_pScanBuffer->vMore(iRead);
  
  return true;

#endif
}

/*----------------------------------------------------------------------------*/
/* insert row                                                                 */
/*----------------------------------------------------------------------------*/
bool Loader2::bInsertRow(i8 iPartn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  ASSERT(iPartn < gxArguments.m_nPartitions);
  
  ValWsp xRow;
  xRow.vClass(CN_STRVAL);
  
  StrVal* pStrVal = xRow.pAsStrVal();
  
  Memory::vCopy(pStrVal->sStr(),m_pRecord,m_iLength);
  pStrVal->sStr()[m_iLength] = '\0';
  pStrVal->vSize();
  
  //vTrace("Row: %s\n",pStrVal->sStr());
  
  m_pTable->vInsertRow(iPartn,m_pTxRef,pStrVal);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get record                                                                 */
/*----------------------------------------------------------------------------*/
void Loader2::vGetRecord(StrVal* pStrVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  Memory::vCopy(pStrVal->sStr(),m_pRecord,m_iLength);
  pStrVal->sStr()[m_iLength] = '\0';
  pStrVal->vSize();
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void Loader2::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pLoadBuffer;
  m_pLoadBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void Loader2::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Loader2::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/