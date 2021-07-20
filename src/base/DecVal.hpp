/*============================================================================*/
/* DecVal.hpp - Decimal Value                                                 */
/*============================================================================*/
#pragma once

namespace BASE
{
  class DecVal
  {
  public:
    i16  m_iValue;   // numeric value
    i8   m_nPlaces;  // number of decimal places

  public:
    DecVal(void);
  
    void operator =(DecVal xDecVal);
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
