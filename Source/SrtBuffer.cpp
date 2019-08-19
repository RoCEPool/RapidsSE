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
  
  if (pRightColVec) {
    
    HV_Rec hvRecRight(pRightColVec);
    HV_Row hRowRight(pRightColVec);
    
    SrtSet* aSrtSet = aFirstSrtSet();
    
    i8 nRows = nItems();
    i8 iElement = 0;
    while (nRows--) {
      
      RASSERT(aSrtSet == this->aSrtSet(iElement));
      
      // get the left dataRow
      Crw* aLeftCrw = (Crw*)(aSrtSet->iRowRef(0));
      
      // inflate the left row
      if (aLeftCrw) {
        RowInflator::vInflateRow(hRowLeft, aLeftCrw);
      } else {
        // get no of columns
        i8 nColumns = pLeftColVec->nItems();
        
        // for each column
        for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
          hRowLeft->m_waColVal[i_Col].m_iI8 = C_DATUM_NULL;
        }
      }
      hvRecLeft->vAppend(hRowLeft);
      
      // get the right dataRow
      Crw* aRightCrw = (Crw*)(aSrtSet->iRowRef(1));
      
      if (aRightCrw) {
        // inflate the right row
        RowInflator::vInflateRow(hRowRight, aRightCrw);
      }
      else {
        // get no of columns
        i8 nColumns = pRightColVec->nItems();
        
        // for each column
        for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
          hRowRight->m_waColVal[i_Col].m_iI8 = C_DATUM_NULL;
        }
      }
      hvRecRight->vAppend(hRowRight);
      
      vTrace("%lld: %p; Key:%lld RowRef:%p %p; row:%s %s\n"_Stz, iElement, aSrtSet, aSrtSet->iKey(), aSrtSet->iRowRef(0),
             aSrtSet->iRowRef(1),
             hvRecLeft->szRecStr(), hvRecRight->szRecStr());
      
      aSrtSet = aNextSrtSet(aSrtSet);
      iElement++;
    }
    
  }
  else {
    SrtSet* aSrtSet = aFirstSrtSet();
    
    i8 nRows = nItems();
    i8 iElement = 0;
    while (nRows--) {
      
      RASSERT(aSrtSet == this->aSrtSet(iElement));
      
      // get the left dataRow
      Crw* aLeftCrw = (Crw*)(aSrtSet->iRowRef(1));
      
      // inflate the left row
      if (aLeftCrw) {
        RowInflator::vInflateRow(hRowLeft, aLeftCrw);
        hvRecLeft->vAppend(hRowLeft);
      }
      
      vTrace("%lld: %p; Key:%lld rowRef:%p;iElement,  row:%s\n"_Stz, aSrtSet, aSrtSet->iKey(), aSrtSet->iRowRef(0),
             hvRecLeft->szRecStr());
      
      aSrtSet = aNextSrtSet(aSrtSet);
      iElement++;
    }
    
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
  
  SrtSet* aSrtSetPtr = aItemArea();
  i8 iSize = sizeof(SrtSet) + sizeof(i8)*(aSrtSetPtr->m_nRowRefs - 1);
  
  aSrtSetPtr = aAdd<SrtSet*>(aSrtSetPtr, iSize*iSrtSet);
  
  if (aSrtSetPtr && CN_SRTSET == aSrtSetPtr->eClass()) {
    return aSrtSetPtr;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return next srt set in this buffer                                         */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vSwap(i8 iSrtSet1, SrtBuffer* pSrtBuffer2, i8 iSrtSet2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtSet* aSrtSet1 = aSrtSet(iSrtSet1);
  SrtSet* aSrtSet2 = pSrtBuffer2->aSrtSet(iSrtSet2);
  
  i8 iKey = aSrtSet1->m_xKey;
  i8 iaRowRef0 = aSrtSet1->m_iaRowRef[0];
  i8 iaRowRef1 = aSrtSet1->m_iaRowRef[1];
  
  aSrtSet1->m_xKey = aSrtSet2->m_xKey;
  aSrtSet1->m_iaRowRef[0] = aSrtSet2->m_iaRowRef[0];
  aSrtSet1->m_iaRowRef[1] = aSrtSet2->m_iaRowRef[1];
  
  aSrtSet2->m_xKey = iKey;
  aSrtSet2->m_iaRowRef[0] = iaRowRef0;
  aSrtSet2->m_iaRowRef[1] = iaRowRef1;
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
