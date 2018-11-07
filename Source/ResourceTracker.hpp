/*============================================================================*/
/* ResourceTracker.hpp - Resource Tracker                                     */
/*============================================================================*/
#pragma once

namespace System
{
  class ResourceTracker : public Object
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    ResourceTracker(void);
    ~ResourceTracker(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
