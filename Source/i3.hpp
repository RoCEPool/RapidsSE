/*============================================================================*/
/* i3.hpp -- a 3-byte integer                                                 */
/*============================================================================*/
#pragma once

namespace Base
{
  class i3
  {
  public:
    i8   m_iValue;
    char m_cValue[3];
  
  public:
    i3(void);
    i3(i8 iArg);
    ~i3(void);
  
    explicit operator i8() const; // { return m_iValue; }
  
    i8 operator *(void);
    void operator =(i8 iArg);
    void vSet(i8 iArg);
  
    bool operator >(i3 iArg);
    bool operator >(i8 iArg);
    bool operator >=(i3 iArg);
    bool operator >=(i8 iArg);
    bool operator ==(i3 iArg);
    bool operator ==(i8 iArg);
    bool operator !=(i3 iArg);
    bool operator !=(i8 iArg);
    bool operator <=(i3 iArg);
    bool operator <=(i8 iArg);
    bool operator <(i3 iArg);
    bool operator <(i8 iArg);
  
    i8 operator +(i3 iArg);
    i8 operator +(i8 iArg);
    i8 operator -(i3 iArg);
    i8 operator -(i8 iArg);
    i8 operator ++(void);
    i8 operator ++(int);
    i8 operator --(void);
    i8 operator --(int);
  
  };
  
  i3 operator "" _i3(u8 uArg);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class i3i8
  {
  public:
    union {
      int  m_iValue;
      char m_cValue[4];
    };
  
    static i8 iToI8(i3* pI3);
    static void vToI3(i3* pI3, i8 iArg);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
