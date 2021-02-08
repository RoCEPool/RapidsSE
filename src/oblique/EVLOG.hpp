/*============================================================================*/
/* EVLOG.hpp -- Event Log                                                     */
/*============================================================================*/
#pragma once

//!                    ** Do not include BASE.h **

#include "eEV.h"

namespace BASE
{
  class Task;
}

namespace STORE
{
  class Job;
  class Session;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace OBLIQUE
{
  class EVLOG
  {
  public:
    static void vEvent(Object* pObject, eEV eEvent, Stz* szFormat, ...);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
