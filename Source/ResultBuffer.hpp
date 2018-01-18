/*============================================================================*/
/* ResultBuffer.hpp -- Result Buffer                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ResultBuffer : public ItemBuffer
  {
  protected:
  bool  m_bIsLast; // is this the last buffer in its sequence?jj
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    ResultBuffer(void);
   ~ResultBuffer(void);
  
    bool bAppendRow(Row* aRow, i8 nRowSize);
  
    void vIsLast(bool bIsLast);
    bool bIsLast(void);
  
    Footer* aFooter(void);
  };
  
  ResultBuffer* _pResultBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
