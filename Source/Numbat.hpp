/*============================================================================*/
/* Numbat.hpp - number analyser                                               */
/*============================================================================*/
#pragma once

namespace Measurment
{
  class Numbat : public Object
  {
  public:
    s32 m_sName;
  
    Timer m_oTimer;
  
    i8  m_iCount;
    i8  m_iMax;
    i8  m_iMin;
    i8  m_iSum;

    d8  m_dM1;
    d8  m_dM2;
    d8  m_dM3;
    d8  m_dM4;

    i8  m_iGrandCount;
    i8  m_iGrandMax;
    i8  m_iGrandMin;
    i8  m_iGrandSum;
  
    d8  m_dGrandM1;
    d8  m_dGrandM2;
    d8  m_dGrandM3;
    d8  m_dGrandM4;

    // must be last
    Footer m_xFooter;
    
  public:
    Numbat(Stz* szName);
    ~Numbat() noexcept(false);
  
    void vAdd(i8 iArg, Stz* szFormat,...);
    void vReset(void);
    void vPrint(Stz* szFormat,...);

    Footer* aFooter(void);
  };
}

using namespace Measurment;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
