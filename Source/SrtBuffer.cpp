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
  
  pSrtBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
RefSet* SrtBuffer::aAllotRefSet(i8 nRowRefs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  // allot using bytes as we don't know the size of the item
  byte* ayRefSet = ayAllotBytes(RefSet::nBytesReq(nRowRefs));
  
  if (! ayRefSet)
    return nullptr;
  
  vIncItems();
  
  RefSet* aRefSet = new(ayRefSet) RefSet(nRowRefs);
  
  return aRefSet;
}

/*----------------------------------------------------------------------------*/
/* return first ref set in this buffer                                        */
/*----------------------------------------------------------------------------*/
RefSet* SrtBuffer::aFirstRefSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  RefSet* aRefSet = aFirstItem();
  
  return aRefSet;
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
/* return next ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
RefSet* SrtBuffer::aNextRefSet(RefSet* aPriorRefSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);

  RefSet* aRefSet = aAdjacentItem(aPriorRefSet);
  
  if (aRefSet && CN_REFSET == aRefSet->eClass())
    return aRefSet;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void SrtBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SRTBUFFER,_FOOTER_);
  
  i8 nRefSets = nItems();
  
  vTrace("SrtBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nRefSets);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
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
