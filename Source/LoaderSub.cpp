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
  void* pObject = gpSmallHeap->pAllocObject(CN_LOADER);
  
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

  m_pLoader    = nullptr;
  m_pRecord     = nullptr;
  m_iLength     = 0;
  m_bMoreChunks = false;
  m_pLoadBuffer = new LoadBuffer;
  
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
/* insert row                                                                 */
/*----------------------------------------------------------------------------*/
bool LoaderSub::bInsertRow(i8 iPartn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADERSUB,_FOOTER_);
  
  ASSERT(iPartn < gxArguments.m_nPartitions);
  
  ValWsp xRow;
  xRow.vClass(CN_STRVAL);
  
  StrVal* pStrVal = xRow.pAsStrVal();
  
  Memory::vCopy(pStrVal->sStr(),m_pRecord,m_iLength);
  pStrVal->sStr()[m_iLength] = '\0';
  pStrVal->vSize();
  
  //vTrace("Row: %s\n",pStrVal->sStr());
  
  bool bResult = m_pLoader->bInsertRow(iPartn,pStrVal);
  
  return true;
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