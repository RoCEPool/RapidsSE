/*============================================================================*/
/* Loader.cpp -- Parallel Table Loader                                        */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;

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
  ASSERT(iSize);
  void* pObject = gpSmallHeap->pAllocObject(CN_LOADER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t iSize, Loader* pLoader)
{
  ASSERT(iSize);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_pTable   = nullptr;
  //m_xFile
  m_iSeek    = 0;
  m_pTxRef   = nullptr;
  //m_xLock
  m_nSubs    = 0;
  
  _checkInitialised_(sizeof(Loader));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
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
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void Loader::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_xFile.vClose();
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
void Loader::vTxRef(TxRef* pTxRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTxRef = pTxRef;
  
  vTrace("TxId %lld\n",pTxRef->iTxId());
  
}

/*----------------------------------------------------------------------------*/
/*-- Called by LoaderSub ----------------------------------------------------*/
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
bool Loader::bInsertRow(i8 iPartn, StrVal* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  ASSERT(iPartn < gxArguments.m_nPartitions);
  
  m_pTable->vInsertRow(iPartn,m_pTxRef,pRow);
  
  return true;
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