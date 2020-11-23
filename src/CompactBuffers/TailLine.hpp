/*============================================================================*/
/* TailLine.hpp -- a tail line                                                */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class TailLine : public Line
  {
  public:
    byte m_yaByte[sizeof(SpaceUnit)];
  
  public:
    TailLine(void);
   ~TailLine(void) noexcept(false);
  
    i8 nBytes(void);
    i8 nUnits(void);
    i8 nWords(void);

    i8   iL(void);
    void vL(i8 nL);
  
    i8   iU(void);
    void vU(i8 nUnits);
  
    void vAssign(Stz* szStz);
    void vDump(void);
    Stz* sz(void);
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline TailLine* _aTailLine(void* aAddr)
  {
    TailLine* aTailLine = reinterpret_cast<TailLine*>(aAddr);
    
    return aTailLine;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
