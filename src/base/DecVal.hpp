/*============================================================================*/
/* DecVal.hpp - Decimal Value                                                 */
/*============================================================================*/
#pragma once

namespace BASE
{
  class DecVal
  {
  public:
                     //                             1000.00  1000.0
    i16  m_iValue;   // numeric value               100000   10000
    i8   m_nPlaces;  // number of decimal places    2        1

  public:
    DecVal(void);
  
    void operator =(DecVal xDecVal);
    DecVal operator +(DecVal xDecVal);
    DecVal operator -(DecVal xDecVal);
    DecVal operator *(DecVal xDecVal);
    DecVal operator /(DecVal xDecVal);

    void vAssign(DecVal xArg);
    void vAssign(i16 iValue, i8 nPlaces);
    void vAssign(i8 iWhole, i8 iFraction, i8 nPlaces);

    void vRoundDown(i8 nPlaces);
    void vRoundUp(i8 nPlaces);
  };

  // user defined string literal
  DecVal operator "" _Dec(const char* azLiteral, size_t nSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
