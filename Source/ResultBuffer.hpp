/*============================================================================*/
/* ResultBuffer.hpp -- Result Buffer                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ResultBuffer : public BufferAbstract
  {
  protected:
  
  // must be last
  Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    ResultBuffer(void);
   ~ResultBuffer(void);
  
  public:
  Footer* aFooter(void);
  };
  
  ResultBuffer* _pResultBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
