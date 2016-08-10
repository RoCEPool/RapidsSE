/*============================================================================*/
/* ObjVal.hpp - object value                                                  */
/*============================================================================*/

#pragma once

#include "StackValues.h"

namespace StackValues
{
  class ObjVal : public OnStack
  {
  private:
    i8   m_iSize;
    
  public:
    void* operator new(size_t);
    
    ObjVal(i8 iSize);
    i8 iSize(void) const;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
