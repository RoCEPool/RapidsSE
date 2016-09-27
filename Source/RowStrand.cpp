/*============================================================================*/
/* RowStrand.cpp -- Row Strand                                                */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* allocate a row strand from small object heap                               */
/*----------------------------------------------------------------------------*/
void* RowStrand::operator new(size_t iSize)
{
  RASSERT(iSize);
  Object* pObject = gpSmallHeap->pAllocObject(CN_ROWSTRAND);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* release a row strand back to the small object heap                         */
/*----------------------------------------------------------------------------*/
void RowStrand::operator delete(void* pRowStrandAddress)
{
  RowStrand* pRowStrand = static_cast<RowStrand*>(pRowStrandAddress);
  
  gpSmallHeap->vFreeObject(pRowStrand);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowStrand::RowStrand(void)
: Object(CN_ROWSTRAND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  m_zLock.vCheckInit();
  m_iRowBuffers  = 0;
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  
  m_pPrevStrand  = nullptr;
  m_pNextStrand  = nullptr;
  
  _checkInitialised_(sizeof(RowStrand));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowStrand::~RowStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump rowbuffer                                                             */
/*----------------------------------------------------------------------------*/
void RowStrand::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_ROWSTRAND);
  ASSERT(pObject->eClass() == CN_ROWSTRAND);
  
  RowStrand* pRowStrand = reinterpret_cast<RowStrand*>(pObject);
  
  vTrace("%sRowStrand %p:\n", pTab, pRowStrand);
  vTrace("%s  rowBuffers %lld\n", pTab, pRowStrand->m_iRowBuffers);
  vTrace("%s  firstBuffer %p\n", pTab, pRowStrand->m_pFirstBuffer);
  vTrace("%s  nextBuffer %p\n", pTab, pRowStrand->m_pLastBuffer);
  vTrace("%s  prevStrand %p\n", pTab, pRowStrand->m_pPrevStrand);
  vTrace("%s  nextStrand %p\n\n", pTab, pRowStrand->m_pNextStrand);
}

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
bool RowStrand::bInsertRow(TxRef* pTxRef, char* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  if (! m_pLastBuffer) {
    if (! bExtendStrand()) {
      m_zLock.vDropWrite();
      return false;
    }
  }

  bool bInserted = m_pLastBuffer->bInsertRow(pTxRef,pRecord,iLength);
  
  if (bInserted) {
    m_zLock.vDropWrite();
    return true;
  }
  
  if (! bExtendStrand()) {
    m_zLock.vDropWrite();
    return false;
  }
  
  bInserted = m_pLastBuffer->bInsertRow(pTxRef,pRecord,iLength);
  ASSERT(bInserted);
  
  m_zLock.vDropWrite();
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  return bInserted;
}

/*----------------------------------------------------------------------------*/
/* get row buffers                                                            */
/*----------------------------------------------------------------------------*/
i8 RowStrand::iRowBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);

  return m_iRowBuffers;
}

/*----------------------------------------------------------------------------*/
/* get first buffer                                                           */
/*----------------------------------------------------------------------------*/
RowBuffer* RowStrand::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  return m_pFirstBuffer;
}

/*----------------------------------------------------------------------------*/
/* get last buffer                                                            */
/*----------------------------------------------------------------------------*/
RowBuffer* RowStrand::pLastBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  return m_pLastBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
RowStrand* RowStrand::pNextStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  return m_pNextStrand;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void RowStrand::vNextStrand(RowStrand* pNextStrand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  m_pNextStrand = pNextStrand;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
RowStrand* RowStrand::pPrevStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  return m_pPrevStrand;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void RowStrand::vPrevStrand(RowStrand* pPrevStrand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  m_pPrevStrand = pPrevStrand;
}

/*----------------------------------------------------------------------------*/
/* release all strands                                                        */
/*----------------------------------------------------------------------------*/
void RowStrand::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  RowBuffer* pRowBuffer = m_pFirstBuffer;
  while (pRowBuffer) {
    RowBuffer* pRowBufferTBD = pRowBuffer;
    pRowBuffer = pRowBuffer->pNextBuffer();
    
    delete pRowBufferTBD;
  }
  
  m_iRowBuffers  = 0;
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  
  m_zLock.vDropWrite();
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extend this strand                                                         */
/*----------------------------------------------------------------------------*/
bool RowStrand::bExtendStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  if (m_iRowBuffers >= C_STRAND_LENGTH)
    return false;

  RowBuffer* pNewBuffer = new RowBuffer;
  
  if (! m_pFirstBuffer) {
    ASSERT(! m_iRowBuffers);
    ASSERT(! m_pLastBuffer);
    
    m_pFirstBuffer = pNewBuffer;
    m_pLastBuffer  = pNewBuffer;
    
    m_iRowBuffers = 1;
  }
  else {
    ASSERT(m_iRowBuffers);
    ASSERT(m_pLastBuffer);
    
    m_pLastBuffer->vNextBuffer(pNewBuffer);
    pNewBuffer->vPrevBuffer(m_pLastBuffer);
    
    m_pLastBuffer = pNewBuffer;
    
    m_iRowBuffers++;
  }
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowStrand::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/