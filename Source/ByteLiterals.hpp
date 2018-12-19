/*============================================================================*/
/* ByteLiterals.hpp -- Byte Literals                                          */
/*============================================================================*/
#pragma once

namespace Base
{
  i8 operator "" _KB(unsigned long long i);
  i8 operator "" _MB(unsigned long long i);
  i8 operator "" _GB(unsigned long long i);
  i8 operator "" _TB(unsigned long long i);
  i8 operator "" _PB(unsigned long long i);
  i8 operator "" _EB(unsigned long long i);

  i8 operator "" _K(unsigned long long i);
  i8 operator "" _M(unsigned long long i);
  i8 operator "" _B(unsigned long long i);
  i8 operator "" _T(unsigned long long i);
  i8 operator "" _P(unsigned long long i);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
