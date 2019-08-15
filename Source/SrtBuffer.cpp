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
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  return _pSrtBuffer(m_pNextBuffer);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  return _pSrtBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vShow(ColumnVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  i8 nSrtSets = nItems();
  
  vTrace("SrtBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nSrtSets);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
  
  HV_Rec hvRec(pColVec);
  HV_Row hRow(pColVec);
  
  SrtSet* aSrtSet = aFirstSrtSet();
  
  while (aSrtSet) {
    
    // get the dataRow
    Crw* aCrw = (Crw*)(aSrtSet->iRowRef(0));
    
    // inflate the row
    RowInflator::vInflateRow(hRow, aCrw);
    hvRec->vAppend(hRow);
    
    vTrace("%p: Key:%lld %p %s\n"_Stz, aSrtSet, aSrtSet->iKey(), aSrtSet->iRowRef(0), hvRec->szRecStr());
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
void SrtBuffer::vSwap(i8 iSrtSet1, i8 iSrtSet2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  SrtSet* aSrtSet = aItemArea();
  
  SrtSet* aSrtSet1 = aAdd<SrtSet*>(aSrtSet, sizeof(SrtSet)*iSrtSet1);
  SrtSet* aSrtSet2 = aAdd<SrtSet*>(aSrtSet, sizeof(SrtSet)*iSrtSet2);
  
  i8 iKey = aSrtSet1->m_xKey;
  i8 iaRowRef = aSrtSet1->m_iaRowRef[0];
  
  aSrtSet1->m_xKey = aSrtSet2->m_xKey;
  aSrtSet1->m_iaRowRef[0] = aSrtSet2->m_iaRowRef[0];
  
  aSrtSet2->m_xKey = iKey;
  aSrtSet2->m_iaRowRef[0] = iaRowRef;
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
