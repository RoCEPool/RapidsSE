/*============================================================================*/
/* Sprint.hpp - Sprint Utility                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class Sprint : public Object
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    Sprint(void);
    ~Sprint() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
