/*============================================================================*/
/* Lrw.hpp -- Linear compact row                                              */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  union LrwCdx
  {
    u8 m_uAsI8;
  
    struct
    {
      u8 m_uLrwBytes : 16; // size of
      u8 m_uRowBytes : 16; // size of
      u8 m_uSpare    : 16; // round up to 64b
      u8 m_uKeyBytes : 16; // size of
    //                ---
    //                64b  //
    //                ===
    };
  };
}

/*----------------------------------------------------------------------------*/

namespace CompactBuffers
{
  class Lrw : public Object
  {
  private:
    LrwCdx m_xLrwCdx;
    m8     m_maLrwBytes[];  // flexible array must be last
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Lrw(void);
   ~Lrw(void) noexcept(false);
  
    LrwCdx* aLrwCdx(void);
    i8 nSize(void);
  
    HeadLine* aHeadLine(void);
    OverLine* aOverLine(void);
    TailLine* aTailLines(void);
  };
  
  Lrw* _pLrw(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
