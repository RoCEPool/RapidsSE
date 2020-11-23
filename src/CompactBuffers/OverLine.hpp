/*============================================================================*/
/* OverLine.hpp -- a overflow line                                            */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class OverLine : public Line
  {
  public:
    byte m_yaByte[sizeof(DoubleWord)]; 

  public:
    OverLine(void);
    ~OverLine(void) noexcept(false);
  
    i8 nBytes(void);
    i8 nUnits(void);
    i8 nWords(void);
  
    void vDump(void);

    i8   iU(void);
    void vU(i8 nUnits);
  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline OverLine* _aOverLine(void* aAddr)
  {
    OverLine* aOverLine = reinterpret_cast<OverLine*>(aAddr);
    
    return aOverLine;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
