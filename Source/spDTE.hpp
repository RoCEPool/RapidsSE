/*============================================================================*/
/* spDTE.hpp - DTE sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class spDTE : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spDTE(void);
    ~spDTE() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
