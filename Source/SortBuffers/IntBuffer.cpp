/*============================================================================*/
/* IntBuffer.cpp -- Integer Buffer                                            */
/*============================================================================*/

#include "SortBuffers.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IntBuffer* SortBuffers::_pIntBuffer(void* aAddress)
{
  IntBuffer* pIntBuffer = reinterpret_cast<IntBuffer*>(aAddress);
  
  RASSERT_TYPE(pIntBuffer,CN_INTBUFFER);
  
  return pIntBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new row buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* IntBuffer::operator new(size_t)
{
  IntBuffer* pIntBuffer =
    static_cast<IntBuffer*>(HeapMem::pAllocate(CN_INTBUFFER));
  
  return pIntBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* IntBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free a row buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void IntBuffer::operator delete(void* aAddress)
{
  IntBuffer* pIntBuffer = static_cast<IntBuffer*>(aAddress);
  
  HeapMem::vFree(pIntBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IntBuffer::IntBuffer(void)
: DataBuffer(CN_INTBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  m_nInts = 0;
  for (i8 i_Index = 0; i_Index < ELEMENTS; i_Index++)
    m_iIntArray[i_Index] = C_UNKNOWN;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_INTBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IntBuffer::~IntBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void IntBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  IntBuffer* pIntBuffer = _pIntBuffer(pObject);
  
  pIntBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return number of row buffers under this buffer                             */
/*----------------------------------------------------------------------------*/
void IntBuffer::vBufferCount(i8& nBuffers)
{
  IntBuffer* pNextBuffer = nullptr;
  
  pNextBuffer = this->pNextBuffer();
  nBuffers++;
  
  while (pNextBuffer) {
    pNextBuffer = pNextBuffer->pNextBuffer();
    nBuffers++;
  }
}

/*----------------------------------------------------------------------------*/
/* append an integer value to the buffer                                      */
/*----------------------------------------------------------------------------*/
void IntBuffer::vAppendValue(i8 iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  m_iIntArray[m_nInts++] = iValue;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void IntBuffer::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
}

/*----------------------------------------------------------------------------*/
/* get the value at index from the buffer                                     */
/*----------------------------------------------------------------------------*/
i8 IntBuffer::iGetValue(i8 i_Int)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  return m_iIntArray[i_Int];
}

/*----------------------------------------------------------------------------*/
/* get the element at index from the buffer                                   */
/*----------------------------------------------------------------------------*/
i8* IntBuffer::aGetElement(i8 i_Int)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  return &m_iIntArray[i_Int];
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool IntBuffer::bHasSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  if (AM::nIndexEntries() > 0) {
    // has space calculation based on no of entries
    if (m_nInts >= AM::nIndexEntries())
      return false;
    else
      return true;
  }
  else {
    // has space calculation based on elements
    if (m_nInts < ELEMENTS)
      return true;
    else
      return false;
  }
}

/*----------------------------------------------------------------------------*/
/* get last buffer and count of buffers                                       */
/*----------------------------------------------------------------------------*/
IntBuffer* IntBuffer::pLastBuffer(i8& iCount)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  IntBuffer* pBuffer = this;
  IntBuffer* pLastBuffer = this;

  iCount = 0;
  while (pBuffer) {
    iCount++;
    pLastBuffer = pBuffer;
    pBuffer = pBuffer->pNextBuffer();
  }
  
  return pLastBuffer;
}

/*----------------------------------------------------------------------------*/
/* link the buffers                                                           */
/*----------------------------------------------------------------------------*/
void IntBuffer::vLinkBuffer(IntBuffer* pIntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  // count no. of linked buffers received
  IntBuffer* pLastBuffer = this;
  IntBuffer* pBuffer = this;
  
  while (pBuffer) {
    pLastBuffer = pBuffer;
    pBuffer = pBuffer->pNextBuffer();
  }

  pLastBuffer->vNextBuffer(pIntBuffer);
  pIntBuffer->vPrevBuffer(pLastBuffer);
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
IntBuffer* IntBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  return _pIntBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void IntBuffer::vNextBuffer(IntBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
IntBuffer* IntBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  return _pIntBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void IntBuffer::vPrevBuffer(IntBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the no of values in the buffer                                         */
/*----------------------------------------------------------------------------*/
i8 IntBuffer::nCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  return m_nInts;
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void IntBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  vTrace("IntBuffer: %p (buffer %lld): values: %lld\n"_Stz,
         this,m_iBufferNo,m_nInts);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("Values:\n"_Stz);
  
  for (i8 i_Row = 0; i_Row < m_nInts; i_Row++)
    vTrace("0x%x m_iIntArray[%lld]: %lld\n"_Stz, &m_iIntArray[i_Row], i_Row, m_iIntArray[i_Row]);
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
  //if (pNextBuffer())
  //  pNextBuffer()->vShow();
}

/*----------------------------------------------------------------------------*/
/* verify the next, previous pointers                                         */
/*----------------------------------------------------------------------------*/
bool IntBuffer::bVerifyNextPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  /*
   Any buffer should have a next whose prev point back to it; any prev should
   point forward to it; a child buffer should point back to its parent
   */
  
  IntBuffer* pIntBuffer = this;
  while (pIntBuffer) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    if (pIntBuffer->pPrevBuffer()) {
      IntBuffer* pPrevBuffer = _pIntBuffer(pIntBuffer->pPrevBuffer());
      if (pPrevBuffer->pNextBuffer() != pIntBuffer) {
        return false;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    if (pIntBuffer->pNextBuffer()) {
      IntBuffer* pNextBuffer = _pIntBuffer(pIntBuffer->pNextBuffer());
      if (pNextBuffer->pPrevBuffer() != pIntBuffer) {
        return false;
      }
    }
    pIntBuffer = pIntBuffer->pNextBuffer();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the order of the integers in this buffer                            */
/*----------------------------------------------------------------------------*/
bool IntBuffer::bVerifyOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER,_FOOTER_);
  
  for (i8 i_Index = 0; i_Index < m_nInts; i_Index++) {
    if (i_Index)
      if (m_iIntArray[i_Index] < m_iIntArray[i_Index-1])
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
Footer* IntBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
