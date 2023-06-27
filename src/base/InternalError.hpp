/*============================================================================*/
/* InternalError.hpp : Internal Error Exception --                            */
/*============================================================================*/
#pragma once

#include "Exception.hpp"

namespace BASE
{
  class InternalError : public LowMemory
  {

  public:
    InternalError(void);

    InternalError(eFK   eFailure,
              const char* azFuncNm,
              const char* azText,
              const char* azFileNm,
              i8    iLineNo);

    ~InternalError(void) noexcept(false);



  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
