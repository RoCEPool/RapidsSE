/*============================================================================*/
/* InProcess.hpp - root class for objects in process space                    */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

namespace StorageEngine
{
  class InProcess : public mn,
                    public cn
  {
  public:
    InProcess(eCN eClass);
    ~InProcess(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
