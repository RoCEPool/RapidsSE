/*============================================================================*/
/* SrtBuffer.cpp -- Reference Buffer                                          */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SrtBuffer* Rows::_pSrtBuffer(void* aAddress)
{
  SrtBuffer* pSrtBuffer = reinterpret_cast<SrtBuffer*>(aAddress);
  
  return pSrtBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new ref buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* SrtBuffer::operator new(size_t)
{
  SrtBuffer* pSrtBuffer =
    static_cast<SrtBuffer*>(HeapMem::pAllocate(CN_SRTBUFFER));
  
  return pSrtBuffer;
}

/*----------------------------------------------------------------------------*/
/* free a ref buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void SrtBuffer::operator delete(void* aAddress)
{
  SrtBuffer* pSrtBuffer = static_cast<SrtBuffer*>(aAddress);
  
  HeapMem::vFree(pSrtBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SrtBuffer::SrtBuffer(i8 iPartition)
: DataBuffer(CN_SRTBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  m_bIsLast = false;
  m_iPartitionNo = iPartition;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_SRTBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SrtBuffer::~SrtBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump reference buffer                                                      */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  SrtBuffer* pSrtBuffer = _pSrtBuffer(pObject);
  
  //pSrtBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a srt set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
SrtSet* SrtBuffer::aAllotSrtSet(i8 nRowRefs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  // allot using bytes as we don't know the size of the item
  byte* aySrtSet = ayAllotBytes(SrtSet::nBytesReq(nRowRefs));
  
  if (! aySrtSet)
    return nullptr;
  
  vIncItems();
  
  SrtSet* aSrtSet = new(aySrtSet) SrtSet(nRowRefs);
  
  return aSrtSet;
}

/*----------------------------------------------------------------------------*/
/* return first srt set in this buffer                                        */
/*----------------------------------------------------------------------------*/
SrtSet* SrtBuffer::aFirstSrtSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtSet* aSrtSet = aFirstItem();
  
  return aSrtSet;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool SrtBuffer::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
SrtBuffer* SrtBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  return _pSrtBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* get last buffer and count of buffers                                       */
/*----------------------------------------------------------------------------*/
SrtBuffer* SrtBuffer::pLastBuffer(i8& iCount)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtBuffer* pBuffer = this;
  SrtBuffer* pLastBuffer = this;
  
  iCount = 0;
  while (pBuffer) {
    iCount++;
    pLastBuffer = pBuffer;
    pBuffer = pBuffer->pNextBuffer();
  }
  
  return pLastBuffer;
}

/*----------------------------------------------------------------------------*/
/* link this buffer to another                                                */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vLinkBuffer(SrtBuffer* pSrtBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  // count no. of linked buffers received
  SrtBuffer* pLastBuffer = this;
  SrtBuffer* pBuffer = this;
  
  while (pBuffer) {
    pLastBuffer = pBuffer;
    pBuffer = pBuffer->pNextBuffer();
  }
  
  pLastBuffer->vNextBuffer(pSrtBuffer);
  pSrtBuffer->vPrevBuffer(pLastBuffer);
}

/*----------------------------------------------------------------------------*/
/* return next srt set in this buffer                                         */
/*----------------------------------------------------------------------------*/
SrtSet* SrtBuffer::aNextSrtSet(SrtSet* aPriorSrtSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);

  SrtSet* aSrtSet = aAdjacentItem(aPriorSrtSet);
  
  if (aSrtSet && CN_SRTSET == aSrtSet->eClass())
    return aSrtSet;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
SrtBuffer* SrtBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  return _pSrtBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vShow(ColumnVec* pLeftColVec, ColumnVec* pRightColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  i8 nSrtSets = nItems();
  
  vTrace("SrtBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nSrtSets);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
  
  HV_Rec hvRecLeft(pLeftColVec);
  HV_Row hRowLeft(pLeftColVec);
  
  HV_Rec hvRecRight(pRightColVec);
  HV_Row hRowRight(pRightColVec);
  
  SrtSet* aSrtSet = aFirstSrtSet();
  
  i8 nRows = nItems();
  while (nRows--) {
    
    // get the left dataRow
    Crw* aLeftCrw = (Crw*)(aSrtSet->iRowRef(0));
    
    // inflate the left row
    if (aLeftCrw) {
      RowInflator::vInflateRow(hRowLeft, aLeftCrw);
      hvRecLeft->vAppend(hRowLeft);
    }
    
    // get the right dataRow
    Crw* aRightCrw = (Crw*)(aSrtSet->iRowRef(0));
  
    if (aRightCrw) {
      // inflate the right row
      RowInflator::vInflateRow(hRowRight, aRightCrw);
      hvRecLeft->vAppend(hRowLeft);
    }
    
    vTrace("%p: Key:%lld %p %s %s\n"_Stz, aSrtSet, aSrtSet->iKey(), aSrtSet->iRowRef(0),
           hvRecLeft->szRecStr(), hvRecRight->szRecStr());
    
    aSrtSet = aNextSrtSet(aSrtSet);
  }
    
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* return next srt set in this buffer                                         */
/*----------------------------------------------------------------------------*/
SrtSet* SrtBuffer::aSrtSet(i8 iSrtSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  if (iSrtSet >= m_nItems)
    return nullptr;
  
  SrtSet* aSrtSet = aItemArea();
  
  aSrtSet = aAdd<SrtSet*>(aSrtSet, sizeof(SrtSet)*iSrtSet);
  
  if (aSrtSet && CN_SRTSET == aSrtSet->eClass()) {
    return aSrtSet;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return next srt set in this buffer                                         */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vSwap(i8 iSrtSet1, SrtBuffer* pSrtBuffer2, i8 iSrtSet2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtSet* aSrtSet1 = aItemArea();
  SrtSet* aSrtSet2 = pSrtBuffer2->aItemArea();
  
  aSrtSet1 = aAdd<SrtSet*>(aSrtSet1, sizeof(SrtSet)*iSrtSet1);
  aSrtSet2 = aAdd<SrtSet*>(aSrtSet2, sizeof(SrtSet)*iSrtSet2);
  
  i8 iKey = aSrtSet1->m_xKey;
  i8 iaRowRef = aSrtSet1->m_iaRowRef[0];
  
  aSrtSet1->m_xKey = aSrtSet2->m_xKey;
  aSrtSet1->m_iaRowRef[0] = aSrtSet2->m_iaRowRef[0];
  
  aSrtSet2->m_xKey = iKey;
  aSrtSet2->m_iaRowRef[0] = iaRowRef;
}

/*----------------------------------------------------------------------------*/
/* verify the next, previous pointers                                         */
/*----------------------------------------------------------------------------*/
bool SrtBuffer::bVerifyNextPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  /*
   Any buffer should have a next whose prev point back to it; any prev should
   point forward to it; a child buffer should point back to its parent
   */
  
  SrtBuffer* pSrtBuffer = this;
  while (pSrtBuffer) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    if (pSrtBuffer->pPrevBuffer()) {
      SrtBuffer* pPrevBuffer = _pSrtBuffer(pSrtBuffer->pPrevBuffer());
      if (pPrevBuffer->pNextBuffer() != pSrtBuffer) {
        return false;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    if (pSrtBuffer->pNextBuffer()) {
      SrtBuffer* pNextBuffer = _pSrtBuffer(pSrtBuffer->pNextBuffer());
      if (pNextBuffer->pPrevBuffer() != pSrtBuffer) {
        return false;
      }
    }
    pSrtBuffer = pSrtBuffer->pNextBuffer();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the order of the integers in this buffer                            */
/*----------------------------------------------------------------------------*/
bool SrtBuffer::bVerifyOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
//  for (i8 i_Index = 0; i_Index < m_nInts; i_Index++) {
//    if (i_Index)
//      if (m_iIntArray[i_Index] < m_iIntArray[i_Index-1])
//        return false;
//  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SrtBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
