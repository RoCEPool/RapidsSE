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

  void* pObject = Heap::pAllocate(CN_ROWSTRAND);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* release a row strand back to the small object heap                         */
/*----------------------------------------------------------------------------*/
void RowStrand::operator delete(void* pRowStrandAddress)
{
  RowStrand* pRowStrand = static_cast<RowStrand*>(pRowStrandAddress);
  
  Heap::vFree(pRowStrand);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowStrand::RowStrand(void)
: Object(CN_ROWSTRAND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  m_zLock.vInit("RowStrand::m_zLock");
  m_nRowBuffers  = 0;
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  
  m_pPrevStrand  = nullptr;
  m_pNextStrand  = nullptr;
  
  _checkInitialised_(CN_ROWSTRAND);
  
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
  
  vTrace("rowBuffers %lld first %p next %p prevStrand %p nextStrand %p\n",
         pRowStrand->m_nRowBuffers,
         pRowStrand->m_pFirstBuffer,
         pRowStrand->m_pLastBuffer,
         pRowStrand->m_pPrevStrand,
         pRowStrand->m_pNextStrand);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append buffer into this strand                                             */
/*----------------------------------------------------------------------------*/
bool RowStrand::bAppendBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  bool bAppended = bExtendStrand(pRowBuffer);

  return bAppended;
}

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
bool RowStrand::bInsertRow(TxRef* pTxRef, char* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  //m_zLock.vGrabWrite();
  
  if (! m_pLastBuffer) {
    if (! bExtendStrand()) {
      m_zLock.vDropWrite();
      return false;
    }
  }

  bool bInserted = m_pLastBuffer->bInsertRow(pTxRef,pRecord,iLength);
  
  if (bInserted) {
    //m_zLock.vDropWrite();
    return true;
  }
  
  if (! bExtendStrand()) {
    //m_zLock.vDropWrite();
    return false;
  }
  
  bInserted = m_pLastBuffer->bInsertRow(pTxRef,pRecord,iLength);
  ASSERT(bInserted);
  
  //m_zLock.vDropWrite();
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  return bInserted;
}

/*----------------------------------------------------------------------------*/
/* get number of rows in strand                                               */
/*----------------------------------------------------------------------------*/
i8 RowStrand::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  i8 nRows = 0;
  for (RowBuffer* pRowBuffer = m_pFirstBuffer;
       pRowBuffer;
       pRowBuffer = pRowBuffer->pNextBuffer())
    nRows += pRowBuffer->nRows();
  
  return nRows;
}

/*----------------------------------------------------------------------------*/
/* get number of row buffers                                                  */
/*----------------------------------------------------------------------------*/
i8 RowStrand::nRowBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);

  return m_nRowBuffers;
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
  
  m_nRowBuffers  = 0;
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
  
  if (m_nRowBuffers >= C_STRAND_LENGTH)
    return false;

  RowBuffer* pNewBuffer = RowBuffer::pNewRowBuffer();
  if (! pNewBuffer) {
    vTrace("RowBuffer allocation failed\n");
    return false;
  }
  
  bool bExtended = bExtendStrand(pNewBuffer);
  return true;
}

/*----------------------------------------------------------------------------*/
/* extend this strand                                                         */
/*----------------------------------------------------------------------------*/
bool RowStrand::bExtendStrand(RowBuffer* pNewRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSTRAND,_FOOTER_);
  
  ASSERT(RD_PERMANENT == pNewRowBuffer->eDurability());
  
  // reached strand length?
  if (m_nRowBuffers >= C_STRAND_LENGTH)
    return false;

  // first buffer in strand?
  if (! m_pFirstBuffer) {
    
    ASSERT(! m_nRowBuffers);
    ASSERT(! m_pLastBuffer);
    
    m_pFirstBuffer = pNewRowBuffer;
    m_pLastBuffer  = pNewRowBuffer;
    
    m_nRowBuffers = 1;
    
    return true;
  }

  ASSERT(m_nRowBuffers);
  ASSERT(m_pLastBuffer);

  // can new buffer be copied into last buffer of this strand?
  i8 iSrcBytesUsed = pNewRowBuffer->iBytesUsed();
  i8 iDstBytesFree = m_pLastBuffer->iFreeSpace();
  if (iSrcBytesUsed <= iDstBytesFree) {

    m_pLastBuffer->vAppendBuffer(pNewRowBuffer);
    
    delete pNewRowBuffer;
    
    return true;
  }
  
  // add buffer to end of strand
  m_pLastBuffer->vNextBuffer(pNewRowBuffer);
  pNewRowBuffer->vPrevBuffer(m_pLastBuffer);
  
  m_pLastBuffer = pNewRowBuffer;
  
  m_nRowBuffers++;
  
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
