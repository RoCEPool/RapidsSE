/*============================================================================*/
/* Loader3Sub.cpp -- Table Loader (sub part)                                  */
/*============================================================================*/

#define USE_LOADBUFFER

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Loader3Sub* DBobjects::_pLoader3Sub(void* pAddr)
{
  Loader3Sub* pLoader = reinterpret_cast<Loader3Sub*>(pAddr);
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader3Sub::operator new(size_t iSize)
{
  ASSERT(iSize);
  void* pObject = gpSmallHeap->pAllocObject(CN_LOADER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader3Sub::operator new(size_t iSize, Loader3Sub* pLoader3Sub)
{
  ASSERT(iSize);
  return pLoader3Sub;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Loader3Sub::operator delete(void* pLoaderAddress)
{
  Loader3Sub* pLoader = reinterpret_cast<Loader3Sub*>(pLoaderAddress);
  
  gpSmallHeap->vFreeObject(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader3Sub::Loader3Sub(void)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pLoader3    = nullptr;
  m_pRecord     = nullptr;
  m_pLoadBuffer = nullptr;
  m_iLength     = 0;
  
  _checkInitialised_(sizeof(Loader3Sub));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader3Sub::~Loader3Sub(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  if (m_pLoadBuffer)
    delete m_pLoadBuffer;
}

/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
void Loader3Sub::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump Loader3Sub                                                            */
/*----------------------------------------------------------------------------*/
void Loader3Sub::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADER);
  
  Loader3Sub* pLoader = reinterpret_cast<Loader3Sub*>(pObject);
  
  vTrace("%Loader3Sub %p:\n", pTab, pLoader);
}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
void Loader3Sub::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read more if necessary                      */
/*----------------------------------------------------------------------------*/
bool Loader3Sub::bFetchRecord(void)
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
bool Loader3Sub::bFetchChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* insert row                                                                 */
/*----------------------------------------------------------------------------*/
bool Loader3Sub::bInsertRow(i8 iPartn)
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
  
  bool bResult = m_pLoader3->bInsertRow(iPartn,pStrVal);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get record                                                                 */
/*----------------------------------------------------------------------------*/
void Loader3Sub::vGetRecord(StrVal* pStrVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  Memory::vCopy(pStrVal->sStr(),m_pRecord,m_iLength);
  pStrVal->sStr()[m_iLength] = '\0';
  pStrVal->vSize();
}

/*----------------------------------------------------------------------------*/
/* set loader                                                                 */
/*----------------------------------------------------------------------------*/
void Loader3Sub::vLoader3(Loader3* pLoader3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pLoader3 = pLoader3;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Loader3Sub::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/