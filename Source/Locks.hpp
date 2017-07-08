/*============================================================================*/
/* Locks.hpp -- lock statistics tracker                                       */
/*============================================================================*/
#pragma once

#include "System.h"

namespace System
{
  class Locks
  {
  public:
    Atomic m_zReadGrabs[LK_ENUMS];       // number of read grabs
    Atomic m_zReadCollisions[LK_ENUMS];  // number of read collisions
    
    Atomic m_zWriteGrabs[LK_ENUMS];      // number of write grabs
    Atomic m_zWriteCollisions[LK_ENUMS]; // number of write collisions
  
    void vList(File& xFile);
    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
