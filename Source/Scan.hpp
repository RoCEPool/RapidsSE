/*============================================================================*/
/* Scan.hpp - Scan Utility                                                    */
/*============================================================================*/
#pragma once

namespace ScanSprint

{
  class Scan : public Object
  {
  public:
  
    i8 m_iValue;
  
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
