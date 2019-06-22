/*============================================================================*/
/* BufferCursor.hpp -- CrwBuffer Cursor                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  class IntBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class BufferCursor : public Object
  {
  public:
    IntBuffer*  m_pIntBuffer;
    i8          m_iBufferNo;
    i8*         m_aElement;

  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
    
    BufferCursor(IntBuffer* pIntBuffer, i8 iBufferNo, i8* aElement);
    BufferCursor(BufferCursor* pBufferCursor);
    BufferCursor(BufferCursor* pLowCursor, BufferCursor* pHighCursor);
    
    ~BufferCursor(void) noexcept(false);
  
    void vMoveLeft(void);
    void vMoveRight(void);
    
    void vSwap(BufferCursor* pBufferCursor);
    
    // is before
    bool bIsBefore(BufferCursor* pBufferCursor)
    {
      if (m_aElement < pBufferCursor->m_aElement)
        return true;
      else
        return false;
    }

    // is after or same
    bool bIsAfterOrSame(BufferCursor* pBufferCursor)
    {
      if (m_iBufferNo > pBufferCursor->m_iBufferNo)
        return true;
      
      if (m_iBufferNo < pBufferCursor->m_iBufferNo)
        return false;
      
      if (m_aElement >= pBufferCursor->m_aElement)
        return true;
      else
        return false;
    }
    
    // less than
    bool bIsLessThan(i8 iValue)
    {
      if (*m_aElement < iValue)
        return true;
      else
        return false;
    }
    
    // greater than
    bool bIsGreaterThan(i8 iValue)
    {
      if (*m_aElement > iValue)
        return true;
      else
        return false;
    }
    
  };
  
  BufferCursor* _pBufferCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
