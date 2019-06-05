/*============================================================================*/
/* Scan.hpp - Scan Utility                                                    */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class Scan : public Object
  {
  public:
  
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
