/*============================================================================*/
/* i3.hpp -- a 3-byte integer                                                 */
/*============================================================================*/
#pragma once

namespace Base
{
  class i3
  {
  private:
    union {
      i8   m_iValue;
      char m_cValue[8];
    };
  
  public:
    i3(void);
    i3(i8 iArg);
    ~i3(void);
  
    i8 operator *(void);
    void operator =(i8 iArg);
  
    bool operator >(i8 iArg);
    bool operator >=(i8 iArg);
    bool operator ==(i8 iArg);
    bool operator !=(i8 iArg);
    bool operator <=(i8 iArg);
    bool operator <(i8 iArg);
  
    i8 operator +(i8 iArg);
    i8 operator -(i8 iArg);
    i8 operator ++(void);
    i8 operator ++(int);
    i8 operator --(void);
    i8 operator --(int);
  };
  
  i3 operator "" _i3(u8 uArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
