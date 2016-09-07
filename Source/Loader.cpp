/*============================================================================*/
/* Loader.cpp -- Table Loader                                                 */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t)
{
  void* pObject = gpSmallHeap->pAllocObject();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t, Loader* pLoader)
{
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Loader::operator delete(void* pLoaderAddress)
{
  Loader* pLoader = reinterpret_cast<Loader*>(pLoaderAddress);
  
  gpSmallHeap->vFreeObject(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader::Loader(void)
: Object(CN_LOADER)
{
  _poolAddress_(this);
  
  m_pTable  = nullptr;
  //m_xFile
  m_pRecord = nullptr;
  m_pScanBuffer = nullptr;
  
  Memory::vCheckInitialised(this,sizeof(Loader));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void)
{
  _poolAddress_(this);
  
  if (m_pScanBuffer)
    delete m_pScanBuffer;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Loader::pTable(void)
{
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Loader::vTable(Table* pTable)
{
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void Loader::vPath(s& sPath)
{
  const char* pPath = sPath.a();
  
  m_xFile.vPath(pPath);
}

/*----------------------------------------------------------------------------*/
/* set txref                                                                  */
/*----------------------------------------------------------------------------*/
void Loader::vTxRef(TxRef* pTxRef)
{
  m_pTxRef = pTxRef;
  
  vTrace("TxId %lld\n",pTxRef->iTxId());
  
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool Loader::bOpenFile(void)
{
  bool b = m_xFile.bOpen(C_OPEN_READ);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
void Loader::vBegin(void)
{
  ASSERT( ! m_pScanBuffer );
  
  m_pScanBuffer = new ScanBuffer;
}

/*----------------------------------------------------------------------------*/
/* dump loader                                                                */
/*----------------------------------------------------------------------------*/
void Loader::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADER);
  
  Loader* pLoader = reinterpret_cast<Loader*>(pObject);
  
  vTrace("%Loader %p:\n", pTab, pLoader);
  vTrace("%s  table %s \n", pTab, pLoader->m_pTable->pTableName());
  vTrace("%s  record %p\n\n", pTab, pLoader->m_pRecord);
  vTrace("%s  record length %lld\n\n", pTab, pLoader->m_iLength);
}

/*----------------------------------------------------------------------------*/
/* fetch next record                                                          */
/*----------------------------------------------------------------------------*/
bool Loader::bFetchRecord(void)
{
  m_pRecord = m_pScanBuffer->pScan(m_iLength);
  if ( m_pRecord )
    return true;
  
  i8 iAvailableBytes;
  char* pInto = m_pScanBuffer->pInto(iAvailableBytes);
  
  i8 iRead = m_xFile.iReadChunk(pInto,iAvailableBytes);
  //vTrace("Read %lld\n",iRead);
  m_pScanBuffer->vMore(iRead);

  m_pRecord = m_pScanBuffer->pScan(m_iLength);
  if ( m_pRecord )
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* insert row                                                                 */
/*----------------------------------------------------------------------------*/
bool Loader::bInsertRow(i8 iPartn)
{
  ASSERT(iPartn < gxArguments.m_iPartitions);
  
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
void Loader::vGetRecord(StrVal* pStrVal)
{
  Memory::vCopy(pStrVal->sStr(),m_pRecord,m_iLength);
  pStrVal->sStr()[m_iLength] = '\0';
  pStrVal->vSize();
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void Loader::vEnd(void)
{
  delete m_pScanBuffer;
  m_pScanBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void Loader::vCloseFile(void)
{
  m_xFile.vClose();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/