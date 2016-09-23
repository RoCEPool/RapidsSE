/*============================================================================*/
/* Loader3.cpp -- Table Loader                                                */
/*============================================================================*/

#define USE_LOADBUFFER

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Loader3* DBobjects::_pLoader3(void* pAddr)
{
  Loader3* pLoader = reinterpret_cast<Loader3*>(pAddr);
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader3::operator new(size_t iSize)
{
  ASSERT(iSize);
  void* pObject = gpSmallHeap->pAllocObject(CN_LOADER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader3::operator new(size_t iSize, Loader3* pLoader)
{
  ASSERT(iSize);
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Loader3::operator delete(void* pLoaderAddress)
{
  Loader3* pLoader = reinterpret_cast<Loader3*>(pLoaderAddress);
  
  gpSmallHeap->vFreeObject(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader3::Loader3(void)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_pTable      = nullptr;
  //m_xFile
  m_iSeek       = 0;
  m_pTxRef      = nullptr;
  //m_xLock
  
  _checkInitialised_(sizeof(Loader3));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader3::~Loader3(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Loader3::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Loader3::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void Loader3::vPath(s& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  const char* pPath = sPath.a();
  
  m_xFile.vPath(pPath);
}

/*----------------------------------------------------------------------------*/
/* set txref                                                                  */
/*----------------------------------------------------------------------------*/
void Loader3::vTxRef(TxRef* pTxRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTxRef = pTxRef;
  
  vTrace("TxId %lld\n",pTxRef->iTxId());
  
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool Loader3::bOpenFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  bool b = m_xFile.bOpen(C_OPEN_READ);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* dump Loader3                                                                */
/*----------------------------------------------------------------------------*/
void Loader3::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADER);
  
  Loader3* pLoader = reinterpret_cast<Loader3*>(pObject);
  
  vTrace("%Loader3 %p:\n", pTab, pLoader);
  vTrace("%s  table %s \n", pTab, pLoader->m_pTable->pTableName());
}

/*----------------------------------------------------------------------------*/
/* fetch next chunk from file                                                 */
/*----------------------------------------------------------------------------*/
bool Loader3::bFetchChunk(LoadBuffer* pLoadBuffer)
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
bool Loader3::bInsertRow(i8 iPartn, StrVal* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  ASSERT(iPartn < gxArguments.m_nPartitions);
  
  m_pTable->vInsertRow(iPartn,m_pTxRef,pRow);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void Loader3::vCloseFile(void)
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
Footer* Loader3::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/