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
  
  m_sPath    = "<none>";
  m_pTable   = nullptr;
  m_pTxRef   = nullptr;
  m_zSubs    = 0;
  m_pLoadMap = new LoadMap;
  
  _checkInitialised_(sizeof(Loader));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pLoadMap;
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
/* get path                                                                   */
/*----------------------------------------------------------------------------*/
s& Loader::sPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  return m_sPath;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void Loader::vPath(s& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_sPath = sPath;
    
  m_pLoadMap->vFilePath(sPath);
}

/*----------------------------------------------------------------------------*/
/* set number of subordinates                                                 */
/*----------------------------------------------------------------------------*/
void Loader::vSubs(i8 nSubs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_zSubs = nSubs;
}

/*----------------------------------------------------------------------------*/
/* subordinate completes, returns true if last one to complete                */
/*----------------------------------------------------------------------------*/
bool Loader::bSubCompleted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  bool bIsDecider;
  
  if (0 == --m_zSubs)
    bIsDecider = true;
  else
    bIsDecider = false;
  
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
  
  bool b = m_pLoadMap->bFindChunk();
  
  return b;
}


/*----------------------------------------------------------------------------*/
/*-- Called by LoaderSub -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER);
  
  for (;;) {
    bool bDone = m_pLoadMap->bDone();
  
    bool bNext = m_pLoadMap->bNextChunk(iSeek,nBytes);
  
    if (bDone && ! bNext)
      return false;
    
    if (bNext)
      break;
    
    //vTrace("Waiting for map\n");
  }
  
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