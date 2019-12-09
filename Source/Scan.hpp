/*============================================================================*/
/* Scan.hpp - Scan Utility                                                    */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class Scan : public Object
  {
  public:
  
    Dtm m_wDtm;
  
    // must be last
    Footer m_xFooter;
    
  public:
    Scan(eCN eClass);
    ~Scan(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
