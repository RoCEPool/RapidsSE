/*============================================================================*/
/* Ring.hpp - Ring                                                            */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Ring
  {
  private:
    i8 m_iAssignment[16];
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Ring(void);
    ~Ring(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/