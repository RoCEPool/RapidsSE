/*============================================================================*/
/* LockTracker.hpp -- lock statistics tracker                                 */
/*============================================================================*/
#pragma once

#include "Measurement.h"

namespace Measurement
{
  class LockTracker
  {
  public:
    Atomic m_ziWorkUnits[LK_ENUMS];       // number of work units (optional)
  
    Atomic m_ziReadGrabs[LK_ENUMS];       // number of read grabs
    Atomic m_ziReadCollisions[LK_ENUMS];  // number of read collisions
    
    Atomic m_ziWriteGrabs[LK_ENUMS];      // number of write grabs
    Atomic m_ziWriteCollisions[LK_ENUMS]; // number of write collisions
  
    void vList(File& oFile);
    void vReport(void);
    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
