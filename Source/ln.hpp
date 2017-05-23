/*============================================================================*/
/*  ln.hpp - level number                                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class on;
  
  class ln
  {
    
  private:
    i8 m_uLevelNo;

  public:
    ln(void);
    ~ln(void);
    
    static ln nZeroLevel(void);
    on nToOffsetNo(void);
    
    void operator =(i8 u);
    bool operator ==(i8 i);
    bool operator !=(i8 i);
    bool operator >=(i8 i);
    bool operator >(i8 i);
    bool operator <=(i8 i);
    bool operator < (i8 i);
    
    i8 operator *(void);
    ln operator +(i8 i);
    ln operator -(i8 i);
    ln operator ++(int);
    ln operator --(int);
  };
  
  ln operator "" _ln(unsigned long long u);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
