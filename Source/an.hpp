/*============================================================================*/
/* an.hpp - atomic number                                                     */
/*============================================================================*/

#pragma once

#include "Types.h"

namespace Types
{
  class an
  {
  private:
    i8 m_iNumber;
    
  public:
    an(void);
    ~an(void);
    
    bool operator ==(i8 n);
    bool operator !=(i8 n);
    bool operator <(i8 n);
    bool operator <=(i8 n);
    bool operator >(i8 n);
    bool operator >=(i8 n);
    
    i8 operator *(void);
    
    i8 operator ++();
    i8 operator --();
    
    i8 operator +(i8 i);
    i8 operator -(i8 i);
   
  private:
    static i8 iAdd(i8* pValue1, i8 iValue2);
    static i8 iSub(i8* pValue1, i8 iValue2);

  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
