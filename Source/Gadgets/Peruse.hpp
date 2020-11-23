/*============================================================================*/
/* Peruse.hpp - number peruser                                                */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  // variables for Welford's Online Algorithm for computing variance
  // refer: https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
  // refer: https://www.johndcook.com/blog/skewness_kurtosis/

  
  /* notes from https://en.wikipedia.org/wiki/Moment_(mathematics):
  In mathematics, a moment is a specific quantitative measure of the shape of a function. It is used in both mechanics and statistics. If the function represents physical density, then the zeroth moment is the total mass, the first moment divided by the total mass is the center of mass, and the second moment is the rotational inertia. If the function is a probability distribution, then the zeroth moment is the total probability (i.e. one), the first moment is the mean, the second central moment is the variance, the third standardized moment is the skewness, and the fourth standardized moment is the kurtosis. The mathematical concept is closely related to the concept of moment in physics.
  */
  

  class Peruse : public Object
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
    Peruse(void);
    Peruse(Stz* szName);
    ~Peruse() noexcept(false);
  
    void vAdd(i8 iArg, Stz* szFormat,...);
    void vReset(void);
    void vPrint(Stz* szFormat,...);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
