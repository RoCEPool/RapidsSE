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
  
  if (AM::nIndexEntries() > 0) {
    if (nItems() == AM::nIndexEntries())
      return nullptr;
  }
  
  // allot using bytes as we don't know the size of the item
  byte* aySrtSet = ayAllotBytes(SrtSet::nBytesReq(nRowRefs));
  
  if (! aySrtSet)
    return nullptr;
  
  vIncItems();
  
  SrtSet* aSrtSet = new(aySrtSet) SrtSet(nRowRefs);
  
  return aSrtSet;
}

/*----------------------------------------------------------------------------*/
/* allot a srt set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
SrtBuffer* SrtBuffer::aCopySrtSet(SrtSet* aSrtSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  i8 nRowRefs = aSrtSet->m_nRowRefs;
  
  // allot using bytes as we don't know the size of the item
  SrtSet* aNewSrtSet = aAllotSrtSet(nRowRefs);
  
  if (! aNewSrtSet) {
    
    // create a new merge SrtBuffer
    SrtBuffer* pSrtBuffer = new SrtBuffer(m_iPartitionNo);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // link this buffer to the new buffer
    this->vNextBuffer(pSrtBuffer);
    pSrtBuffer->vPrevBuffer(this);
    
    aNewSrtSet = pSrtBuffer->aAllotSrtSet(nRowRefs);
    
    // copy left value to new buffer
    aNewSrtSet->vCopy(aSrtSet);
    
    // return the new buffer
    return pSrtBuffer;
  }
  
  // copy left value to new buffer
  aNewSrtSet->vCopy(aSrtSet);
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* get no of items in the srtBuffer and linked buffers                        */
/*----------------------------------------------------------------------------*/
i8 SrtBuffer::nItemCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtBuffer* pSrtBuffer = this;
  
  i8 iCount = 0;
  while (pSrtBuffer) {
    iCount += pSrtBuffer->nItems();
    pSrtBuffer = pSrtBuffer->pNextBuffer();
  }
  
  return iCount;
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
/* get keys into the srtBuffer                                                */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vGetKeys(i8 iKeyCol, bool bKeyIsLeft,
                         ColumnVec* pLeftColVec,
                         ColumnVec* pRightColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtSet* pSrtSet;
  
  CrwBuffer* pLeftCrwBuffer;
  CrwBuffer* pRightCrwBuffer;
  
  Crw* aLeftCrw;
  Crw* aRightCrw;
  
  SrtSet* aSrtSet = aFirstSrtSet();
  while (aSrtSet) {
    
    // get the left data row
    aLeftCrw = (Crw*)aSrtSet->iRowRef(0);
    
    // set the rowData in the srtSet
    aSrtSet->vRowRef(0, (i8)aLeftCrw);
    
    // get the right dataRow
    aRightCrw = (Crw*)aSrtSet->iRowRef(1);
    
    // set the rowData in the srtSet
    aSrtSet->vRowRef(1, (i8)aRightCrw);
    
    if ( bKeyIsLeft) {
      HV_Row hRow(pLeftColVec);
      
      // get the key from the left buffer row
      if (aLeftCrw) {
        RowInflator::vInflateColumn(hRow, aLeftCrw, iKeyCol);
        
        // set the key in the srtSet
        aSrtSet->vKey(hRow->m_waColVal[iKeyCol].m_iI8);
      }
      else
        aSrtSet->vKey(C_DATUM_NULL);
    }
    else {
      HV_Row hRow(pRightColVec);
      
      // get the key from the left buffer row
      if (aRightCrw) {
        RowInflator::vInflateColumn(hRow, aRightCrw, iKeyCol);
        
        // set the key in the srtSet
        aSrtSet->vKey(hRow->m_waColVal[iKeyCol].m_iI8);
      }
      else
        aSrtSet->vKey(C_DATUM_NULL);
    }
    
    aSrtSet = aNextSrtSet(aSrtSet);
  }
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
/* get no of items in the srtBuffer and linked buffers                        */
/*----------------------------------------------------------------------------*/
i8 SrtBuffer::nBufferCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtBuffer* pSrtBuffer = this;
  
  i8 iCount = 0;
  while (pSrtBuffer) {
    iCount ++;
    pSrtBuffer = pSrtBuffer->pNextBuffer();
  }
  
  return iCount;
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
void SrtBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  i8 nSrtSets = nItems();
  //vTrace ("Rows: %lld\n"_Stz, nSrtSets);
  
  vTrace("SrtBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nSrtSets);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
    
  SrtSet* aSrtSet = aFirstSrtSet();
    
  i8 nRows = nItems();
  i8 iElement = 0;
  
  while (aSrtSet) {
      
    RASSERT(aSrtSet == this->aSrtSet(iElement));
    vTrace("SrtSet %lld: iKey: %lld %p\n"_Stz, iElement, aSrtSet->iKey(), aSrtSet);
    aSrtSet = aNextSrtSet(aSrtSet);
    iElement++;
    
  }
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vShow(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  i8 nSrtSets = nItems();
  //vTrace ("Rows: %lld\n"_Stz, nSrtSets);
  
  vTrace("SrtBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nSrtSets);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
  
  SrtSet* aSrtSet = aFirstSrtSet();
  
  i8 nRows = nItems();
  i8 iElement = 0;
  while (nRows--) {
    
    RASSERT(aSrtSet == this->aSrtSet(iElement));
    
    i8 nColVecs = pStreamDef->m_oColVecAry.nItems();
    RASSERT(nColVecs == aSrtSet->nRowRefs());
    
    vTrace("%lld: %p; Key:%lld; RowRef:"_Stz, iElement, aSrtSet, aSrtSet->iKey());
    
    for (i8 iRowRefs = 0; iRowRefs < aSrtSet->nRowRefs(); iRowRefs++) {
      vTrace("%p "_Stz, aSrtSet->iRowRef(iRowRefs));
    }
    
    vTrace("; row: "_Stz);
    
    for (i8 iRowRefs = 0; iRowRefs < aSrtSet->nRowRefs(); iRowRefs++) {
      
      // get the srtSet dataRow
      Crw* aCrw = (Crw*)(aSrtSet->iRowRef(iRowRefs));
      
      // get the columnVec defining the srtSet
      ColumnVec* pStreamColVec = pStreamDef->m_oColVecAry.pGet(iRowRefs);
      
      HV_Rec hvRec(pStreamColVec);
      HV_Row hRow(pStreamColVec);

      // inflate the row
      if (aCrw) {
        RowInflator::vInflateRow(hRow, aCrw);
      } else {
        // get no of columns
        i8 nColumns = pStreamColVec->nItems();
        
        // for each column
        for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
          hRow->m_waColVal[i_Col].m_iI8 = C_DATUM_NULL;
        }
      }
      hvRec->vAppend(hRow);
      
      vTrace("%s"_Stz,hvRec->szRecStr());
    }
    
    vTrace("\n"_Stz);
    
    aSrtSet = aNextSrtSet(aSrtSet);
    iElement++;
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
  
  if ( aSrtSet1->m_nRowRefs == 1) {
    RASSERT(aSrtSet2->m_nRowRefs == 1);
    
    i8 iKey = aSrtSet1->m_xKey;
    i8 iaRowRef0 = aSrtSet1->m_iaRowRef[0];
    
    aSrtSet1->m_xKey = aSrtSet2->m_xKey;
    aSrtSet1->m_iaRowRef[0] = aSrtSet2->m_iaRowRef[0];
    
    aSrtSet2->m_xKey = iKey;
    aSrtSet2->m_iaRowRef[0] = iaRowRef0;
  }
  else {
    RASSERT(aSrtSet1->m_nRowRefs == 2);
    RASSERT(aSrtSet2->m_nRowRefs == 2);
    
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
