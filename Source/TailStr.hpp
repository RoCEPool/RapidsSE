/*============================================================================*/
/* TailStr.hpp -- a string tail line prototype                                */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class TailStr : public Line
  {
  public:
    byte m_yaByte[C_MAX_VCH + sizeof(SingleWord)];
  
  public:
    TailStr(void);
    TailStr(Stz* szStz);
    ~TailStr(void) noexcept(false);
  
    i8 nBytes(void);
    i8 nUnits(void);
    i8 nWords(void);

    i8   iL(void);
    void vL(i8 nL);
  
    i8   iU(void);
    void vU(i8 nUnits);
  
    // - - - - - - - - -
  
    void vAssign(Stz* szStz);
    void vSet(Stz* szStz);
    Stz* sz(void);
    void vDump(void);
    void vReset(void);
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline TailStr* _aTailStr(void* aAddr)
  {
    TailStr* aTailStr = reinterpret_cast<TailStr*>(aAddr);
    
    return aTailStr;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
