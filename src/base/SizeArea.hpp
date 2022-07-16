/*============================================================================*/
/* SizeArea.hpp -- Size + Area                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace BASE
{
  static const i8 C_SIZE_AREA = 448;

  class SizeArea
  {
  public:
    i8    m_jStrSize;
    char  m_caArea[C_SIZE_AREA];

  public:
    SizeArea(void);
   ~SizeArea(void) noexcept(false);
  
    // auto conversion
    operator SizeArea*(void);

    char* azArea(void);
    Stz*  szArea(void);

    i8    jStrSize(void);
  };

  SizeArea* _pSizeArea(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
