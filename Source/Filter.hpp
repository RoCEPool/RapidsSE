/*============================================================================*/
/* Filter.hpp -- Filter                                                       */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Filter : public Operand
  {
  protected:
    TokenVec* m_pTokenVec;
  
  public:
    void operator delete(void*);
  
    Filter(eCN eClass, TokenVec* pTokenVec=nullptr);
   ~Filter(void) noexcept(false);
  };

  Filter* _pFilter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
