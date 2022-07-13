/*============================================================================*/
/* TypeValue.hpp -- Type + Value                                              */
/*============================================================================*/
#pragma once

#include "eSI.h"

namespace BASE
{
  class TypeValue
  {
  public:
    _Header m_wxType;
    Dtm     m_wxValue;


  public:
    TypeValue(void);
   ~TypeValue(void) noexcept(false);
  
    // auto conversion
    operator TypeValue*(void);

  
  private:
  
  };

  TypeValue* _pTypeValue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
