/*============================================================================*/
/* Ring.hpp - Ring                                                            */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Ring
  {
    static const i8 C_ASSIGNMENT = 16;

  private:
    i8 m_iAssignment[C_ASSIGNMENT];
    
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