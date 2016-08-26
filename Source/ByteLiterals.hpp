/*============================================================================*/
/* ByteLiterals.hpp -- Byte Literals                                          */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Common
{
  i8 operator "" _KB(unsigned long long i);
  i8 operator "" _MB(unsigned long long i);
  i8 operator "" _GB(unsigned long long i);
  i8 operator "" _TB(unsigned long long i);
  i8 operator "" _PB(unsigned long long i);
  i8 operator "" _EB(unsigned long long i);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/