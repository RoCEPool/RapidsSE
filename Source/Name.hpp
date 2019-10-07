/*============================================================================*/
/* Name.hpp -- Name (of object instance)                                      */
/*============================================================================*/
#pragma once

namespace Structures
{
  class Name : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    Name(void);
   ~Name(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
