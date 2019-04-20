/*============================================================================*/
/* Peruse.hpp - number peruser                                                */
/*============================================================================*/
#pragma once

namespace Measurment
{
  // variables for Welford's Online Algorithm for computing variance
  // refer: https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
  
  class Peruse : public Object
  {
  public:
    s32 m_sName;
  
    i8  m_iCount;
    i8  m_iMax;
    i8  m_iMin;
    i8  m_iSum;

    i8  m_iMean;
    i8  m_iMeanSqr;
  
    i8  m_iGrandCount;
    i8  m_iGrandMax;
    i8  m_iGrandMin;
    i8  m_iGrandSum;
  
    i8  m_iGrandMean;
    i8  m_iGrandMeanSqr;
  
    // must be last
    Footer m_xFooter;
    
  public:
    Peruse(Stz* szName);
    ~Peruse() noexcept(false);
  
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
