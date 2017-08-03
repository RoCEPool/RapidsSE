/*============================================================================*/
/* EbStrand.cpp -- Entry Buffer Strand                                        */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* allocate a row strand from small object heap                               */
/*----------------------------------------------------------------------------*/
void* EbStrand::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_EBSTRAND);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* release a row strand back to the small object heap                         */
/*----------------------------------------------------------------------------*/
void EbStrand::operator delete(void* pEbStrandAddress)
{
  EbStrand* pEbStrand = static_cast<EbStrand*>(pEbStrandAddress);
  
  Heap::vFree(pEbStrand);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EbStrand::EbStrand(void)
: Object(CN_EBSTRAND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  m_zLock.vInitLock(LK_EB_STRAND);
  
  m_nEntryBuffers  = 0;
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  
  m_pPrevStrand  = nullptr;
  m_pNextStrand  = nullptr;
  
  _checkInitialised_(CN_EBSTRAND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EbStrand::~EbStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump rowbuffer                                                             */
/*----------------------------------------------------------------------------*/
void EbStrand::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_EBSTRAND);
  ASSERT(pObject->eClass() == CN_EBSTRAND);
  
  EbStrand* pEbStrand = reinterpret_cast<EbStrand*>(pObject);
  
  vTrace("rowBuffers %lld first %p next %p prevStrand %p nextStrand %p\n"_Stz,
         pEbStrand->m_nEntryBuffers,
         pEbStrand->m_pFirstBuffer,
         pEbStrand->m_pLastBuffer,
         pEbStrand->m_pPrevStrand,
         pEbStrand->m_pNextStrand);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append buffer into this strand                                             */
/*----------------------------------------------------------------------------*/
bool EbStrand::bAppendBuffer(EntryBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  bool bAppended = bExtendStrand(pRowBuffer);

  return bAppended;
}

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
bool EbStrand::bInsertRow(TxRef* pTxRef, Stz* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  //m_zLock.vGrabWrite();
  Entry* pRow;
  
  if (! m_pLastBuffer) {
    if (! bExtendStrand()) {
      m_zLock.vDropWrite();
      return false;
    }
  }

  // create working row
  EntWrk xRowWrk;
  xRowWrk.vTxId(pTxRef->iTxId());
  xRowWrk.vData(pRecord, iLength);
  bool bInserted = m_pLastBuffer->bInsertRow(xRowWrk, &pRow);
  
  if (bInserted) {
    //m_zLock.vDropWrite();
    return true;
  }
  
  if (! bExtendStrand()) {
    //m_zLock.vDropWrite();
    return false;
  }
  
  bInserted = m_pLastBuffer->bInsertRow(xRowWrk, &pRow);
  ASSERT(bInserted);
  
  //m_zLock.vDropWrite();
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  return bInserted;
}

/*----------------------------------------------------------------------------*/
/* get number of rows in strand                                               */
/*----------------------------------------------------------------------------*/
i8 EbStrand::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  i8 nRows = 0;
  for (EntryBuffer* pRowBuffer = m_pFirstBuffer;
       pRowBuffer;
       pRowBuffer = (EntryBuffer*)pRowBuffer->pNextBuffer())
    nRows += pRowBuffer->nEntries();
  
  return nRows;
}

/*----------------------------------------------------------------------------*/
/* get number of entry buffers                                                */
/*----------------------------------------------------------------------------*/
i8 EbStrand::nEntryBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);

  return m_nEntryBuffers;
}

/*----------------------------------------------------------------------------*/
/* get first buffer                                                           */
/*----------------------------------------------------------------------------*/
EntryBuffer* EbStrand::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  return m_pFirstBuffer;
}

/*----------------------------------------------------------------------------*/
/* get last buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EbStrand::pLastBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  return m_pLastBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EbStrand* EbStrand::pNextStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  return m_pNextStrand;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EbStrand::vNextStrand(EbStrand* pNextStrand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  m_pNextStrand = pNextStrand;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EbStrand* EbStrand::pPrevStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  return m_pPrevStrand;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EbStrand::vPrevStrand(EbStrand* pPrevStrand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  m_pPrevStrand = pPrevStrand;
}

/*----------------------------------------------------------------------------*/
/* release all strands                                                        */
/*----------------------------------------------------------------------------*/
void EbStrand::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  EntryBuffer* pRowBuffer = m_pFirstBuffer;
  while (pRowBuffer) {
    EntryBuffer* pRowBufferTBD = pRowBuffer;
    pRowBuffer = (EntryBuffer*)pRowBuffer->pNextBuffer();
    
    delete pRowBufferTBD;
  }
  
  m_nEntryBuffers  = 0;
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  
  m_zLock.vDropWrite();
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extend this strand                                                         */
/*----------------------------------------------------------------------------*/
bool EbStrand::bExtendStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  if (m_nEntryBuffers >= C_STRAND_LENGTH)
    return false;

  EntryBuffer* pNewBuffer = (EntryBuffer*)EntryBuffer::pNewEntryBuffer(CN_ROWBUFFER);
  if (! pNewBuffer) {
    vTrace("EntryBuffer allocation failed\n"_Stz);
    return false;
  }
  
  bool bExtended = bExtendStrand(pNewBuffer);
  return true;
}

/*----------------------------------------------------------------------------*/
/* extend this strand                                                         */
/*----------------------------------------------------------------------------*/
bool EbStrand::bExtendStrand(EntryBuffer* pNewRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND,_FOOTER_);
  
  ASSERT(BD_PERMANENT == pNewRowBuffer->eDurability());
  
  // reached strand length?
  if (m_nEntryBuffers >= C_STRAND_LENGTH)
    return false;

  // first buffer in strand?
  if (! m_pFirstBuffer) {
    
    ASSERT(! m_nEntryBuffers);
    ASSERT(! m_pLastBuffer);
    
    m_pFirstBuffer = pNewRowBuffer;
    m_pLastBuffer  = pNewRowBuffer;
    
    m_nEntryBuffers = 1;
    
    return true;
  }

  ASSERT(m_nEntryBuffers);
  ASSERT(m_pLastBuffer);

  // can new buffer be copied into last buffer of this strand?
  i8 iSrcBytesUsed = pNewRowBuffer->nUsedBytes();
  i8 iDstBytesFree = m_pLastBuffer->nFreeBytes();
  if (iSrcBytesUsed <= iDstBytesFree) {

    m_pLastBuffer->vAppendBuffer(pNewRowBuffer);
    
    delete pNewRowBuffer;
    
    return true;
  }
  
  // add buffer to end of strand
  m_pLastBuffer->vNextBuffer(pNewRowBuffer);
  pNewRowBuffer->vPrevBuffer(m_pLastBuffer);
  
  m_pLastBuffer = pNewRowBuffer;
  
  m_nEntryBuffers++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EbStrand::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EBSTRAND);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
