/*============================================================================*/
/* TailLine.hpp -- a tail line                                                */
/*============================================================================*/
#pragma once

namespace Base
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
