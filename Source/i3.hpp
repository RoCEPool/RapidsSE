/*============================================================================*/
/* i3.hpp -- a 3-byte integer                                                 */
/*============================================================================*/
#pragma once

namespace Base
{
  class i3
  {
  public:
    unsigned char m_caValue[3];
  
  public:
    i3(void);
    i3(i8 iArg);
    ~i3(void) noexcept(false);
  
    explicit operator i8(); // { return m_iValue; }
  
    i8 operator *(void);
    void operator =(i8 iArg);

    i8   iGet(void);
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
      unsigned int  m_iValue;
      unsigned char m_caValue[4];
    };
  
    static i8 iToI8(i3* pI3);
    static void vToI3(i3* pI3, i8 iArg);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
