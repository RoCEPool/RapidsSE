/*============================================================================*/
/* Peruse.hpp - number peruser                                                */
/*============================================================================*/
#pragma once

namespace Measurment
{
  class Peruse : public Object
  {
  public:
    s32 m_sName;
    i8 m_iCount;
  
    // must be last
    Footer m_xFooter;
    
  public:
    Peruse(Stz* szName);
    ~Peruse() noexcept(false);
  
    void vInc(Stz* szFormat,...);
        
    Footer* aFooter(void);
  };
}
using namespace Measurment;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
