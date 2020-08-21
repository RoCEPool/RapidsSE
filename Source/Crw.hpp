/*============================================================================*/
/* Crw.hpp -- Compact Row                                                     */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class Crw : public HeadLine
  {
  public:

  public:
    Crw(void);
   ~Crw(void) noexcept(false);
  
    static void vDumpCrw(Stz* szTitle, HeadLine* aHeadLine, StreamDef* pStreamDef);
    static void vDumpCrw_v1(Stz* szTitle, HeadLine* aHeadLine, StreamDef* pStreamDef);
    static void vDumpCrw_v2(Stz* szTitle, HeadLine* aHeadLine, StreamDef* pStreamDef);
  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Crw* _aCrw(void* aAddr)
  {
    Crw* aCrw = reinterpret_cast<Crw*>(aAddr);
    
    return aCrw;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
