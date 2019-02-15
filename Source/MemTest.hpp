/*============================================================================*/
/* MemTest.hpp -- Memory pattern test                                         */
/*============================================================================*/
#pragma once

namespace System
{
  class MemTest : public Object
  {
  public:

    // must be last
    Footer m_xFooter;
  
    void* operator new(size_t);
    void operator delete(void* aAddr);

    MemTest(void);
    ~MemTest(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  MemTest* _pMemTest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
