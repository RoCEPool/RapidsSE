/*============================================================================*/
/* User.hpp - User                                                            */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class User : public Object
  {
  private:
    s32   sName;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/