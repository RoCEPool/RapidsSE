/*============================================================================*/
/* SrtBuffer.cpp -- Reference Buffer                                          */
/*============================================================================*/

#include "SortBuffers.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SrtBuffer* SortBuffers::_pSrtBuffer(void* aAddress)
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
SrtBuffer::SrtBuffer(i8 nRowRefs, i8 iPartition)
: DataBuffer(CN_SRTBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  m_nRowRefs = nRowRefs;
  m_bIsLast = false;
  m_iPartitionNo = iPartition;
  m_nSrtSetBytes = sizeof(SrtSet) - sizeof(i8) + (m_nRowRefs * sizeof(i8));
  
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
SrtSet* SrtBuffer::aAllotSrtSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  if (AM::nIndexEntries() > 0) {
    if (nItems() == AM::nIndexEntries())
      return nullptr;
  }
  
  // allot using bytes as we don't know the size of the item
  byte* aySrtSet = ayAllotBytes(m_nSrtSetBytes);
  
  if (! aySrtSet)
    return nullptr;
  
  vIncItems();
  
  SrtSet* aSrtSet = new(aySrtSet) SrtSet(m_nRowRefs);
  
  return aSrtSet;
}

/*----------------------------------------------------------------------------*/
/* allot a srt set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
SrtBuffer* SrtBuffer::aCopySrtSet(SrtSet* aSrtSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  // allot using bytes as we don't know the size of the item
  SrtSet* aNewSrtSet = aAllotSrtSet();
  
  if (! aNewSrtSet) {
    
    // create a new merge SrtBuffer
    SrtBuffer* pSrtBuffer = new SrtBuffer(m_nRowRefs, m_iPartitionNo);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // link this buffer to the new buffer
    this->vNextBuffer(pSrtBuffer);
    pSrtBuffer->vPrevBuffer(this);
    
    aNewSrtSet = pSrtBuffer->aAllotSrtSet();
    
    // copy left value to new buffer
    aNewSrtSet->vCopy(aSrtSet, m_nRowRefs);
    
    // return the new buffer
    return pSrtBuffer;
  }
  
  // copy left value to new buffer
  aNewSrtSet->vCopy(aSrtSet, m_nRowRefs);
  
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
void SrtBuffer::vGetKeys(StreamDef* pStreamDef, i8 iKeyCol, i8 iColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  Crw* aCrw;

  HV_Row hRow(pStreamDef,C_ALL);
  
  SrtSet* aSrtSet = aFirstSrtSet();
  while (aSrtSet) {
    
      // get the dataRow
      aCrw = (Crw*)aSrtSet->iRowRef(iColVec);
      
      // get the key from the left buffer row
      if (aCrw) {
        RowInflator::vInflateColumn_v1(hRow, aCrw, iKeyCol);
        
        // set the key in the srtSet
        aSrtSet->vKey(hRow->m_waColVal[iKeyCol].m_iWrd);
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
  
  SrtSet* aSrtSet = aAdjacentItem(aPriorSrtSet, m_nSrtSetBytes);
  
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
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vPrintRows(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtSet* aSrtSet = aFirstSrtSet();
  
  // NOTE: We need to iterate nItems() rows as its possible
  // that limit has truncated the no of rows
  for (i8 i_Row = 0; i_Row < nItems(); i_Row++) {
    
    vTrace("row: ("_Stz);
    
    for (i8 iRowRef = 0; iRowRef < m_nRowRefs; iRowRef++) {
      
      if (iRowRef != 0)
        vTrace(" "_Stz);
      
      // get the dataRow of rowref
      Crw* aCrw = (Crw*)(aSrtSet->iRowRef(iRowRef));
      
      HV_Rec hvRec(pStreamDef, iRowRef);
      HV_Row hRow(pStreamDef, iRowRef);
      
      // inflate the row
      if (aCrw) {
        RowInflator::vInflateRow_v1(hRow, aCrw);
      } else {
        // get no of columns
        i8 nColumns = pStreamDef->nColumns(iRowRef);
        
        // for each column
        for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
          hRow->m_waColVal[i_Col].vSetNull();
        }
      }
      hvRec->vAppend(hRow);
      hvRec->vPrintPartialRow();
    }
    
    vTrace(")\n"_Stz);
    
    aSrtSet = aNextSrtSet(aSrtSet);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get the number of row refs in this srt set                                 */
/*----------------------------------------------------------------------------*/
i8 SrtBuffer::nRowRefs(void)
{
  return m_nRowRefs;
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
    
    RASSERT(pStreamDef->m_nDefSets == m_nRowRefs);
    
    vTrace("%lld: %p; Key:%lld; RowRef:"_Stz, iElement, aSrtSet, aSrtSet->iKey());
    
    for (i8 iRowRefs = 0; iRowRefs < m_nRowRefs; iRowRefs++) {
      vTrace("%p "_Stz, aSrtSet->iRowRef(iRowRefs));
    }
    
    vTrace("; row: "_Stz);
  
    for (i8 iRowRefs = 0; iRowRefs < m_nRowRefs; iRowRefs++) {
      
      // get the srtSet dataRow
      Crw* aCrw = (Crw*)(aSrtSet->iRowRef(iRowRefs));
      
      HV_Rec hvRec(pStreamDef, iRowRefs);
      HV_Row hRow(pStreamDef, iRowRefs);

      // inflate the row
      if (aCrw) {
        RowInflator::vInflateRow_v1(hRow, aCrw);
      } else {
        // get no of columns
        i8 nColumns = pStreamDef->nColumns(iRowRefs);
        
        // for each column
        for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
          hRow->m_waColVal[i_Col].vSetNull();
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
  
  SrtSet* aSrtSetPtr = aItemArray();
  i8 iSize = sizeof(SrtSet) + sizeof(i8)*(m_nRowRefs - 1);
  
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
  
  if ( m_nRowRefs == 1) {
    RASSERT(m_nRowRefs == 1);
    
    i8 iKey = aSrtSet1->m_xKey;
    i8 iaRowRef0 = aSrtSet1->m_iaRowRef[0];
    
    aSrtSet1->m_xKey = aSrtSet2->m_xKey;
    aSrtSet1->m_iaRowRef[0] = aSrtSet2->m_iaRowRef[0];
    
    aSrtSet2->m_xKey = iKey;
    aSrtSet2->m_iaRowRef[0] = iaRowRef0;
  }
  else {
    RASSERT(m_nRowRefs == 2);
    
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
/* return next srt set in this buffer                                         */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vSwap(i8 iSrtSet1, i8 iSrtSet2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtSet* aSrtSet1 = aSrtSet(iSrtSet1);
  SrtSet* aSrtSet2 = aSrtSet(iSrtSet2);
  
  if ( m_nRowRefs == 1) {
    RASSERT(m_nRowRefs == 1);
    
    i8 iKey = aSrtSet1->m_xKey;
    i8 iaRowRef0 = aSrtSet1->m_iaRowRef[0];
    
    aSrtSet1->m_xKey = aSrtSet2->m_xKey;
    aSrtSet1->m_iaRowRef[0] = aSrtSet2->m_iaRowRef[0];
    
    aSrtSet2->m_xKey = iKey;
    aSrtSet2->m_iaRowRef[0] = iaRowRef0;
  }
  else {
    RASSERT(m_nRowRefs == 2);
    RASSERT(m_nRowRefs == 2);
    
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
  
  i8 iItems = nItems();
  for (i8 i_Index = 0; i_Index < iItems-1; i_Index++) {
    SrtSet* aSrtSet1 = aSrtSet(i_Index);
    SrtSet* aSrtSet2 = aSrtSet(i_Index+1);
    if (aSrtSet1->iKey() > aSrtSet2->iKey())
      return false;
  }
  
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
