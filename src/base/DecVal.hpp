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
    bool m_bScaled;  // is the value in scaled form?

  public:
     DecVal(void);
  
    void vAssign(i16 iValue,i8 nPlaces, bool bScaled);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
