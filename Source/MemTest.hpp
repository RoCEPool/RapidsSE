/*============================================================================*/
/* MemTest.hpp -- Memory pattern test                                         */
/*============================================================================*/
#pragma once

namespace System
{
  class MemTest : public Object
  {
  public:

    i8   m_iInt;
    i8   m_iIntFlipped;
    i8   m_iNull;
    i8   m_iNullFlipped;
    char m_caCharA[8];
    char m_caCharB[8];
    char m_caCharC[8];

    char m_caCharUpper16[16];
    i8   m_iCharUpperFlipA;
    i8 m_iCharUpperFlipB;

    char m_caCharLower16[16];
    i8   m_iCharLowerFlipA;
    i8 m_iCharLowerFlipB;
  
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
