/*============================================================================*/
/* ln.hpp - level number                                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class on;
  
  class ln
  {
    
  private:
    i8 m_iLevelNo;

  public:
    ln(void);
    explicit ln(i8 i);
    ~ln(void);
    
  operator i8(); // auto conversion to i8
  
    static ln nZeroLevel(void);
  //on nToOffsetNo(void);
    
    void operator =(i8 i);
    bool operator ==(i8 i);
    bool operator !=(i8 i);
    bool operator >=(i8 i);
    bool operator >(i8 i);
    bool operator <=(i8 i);
    bool operator < (i8 i);
    
    void operator =(ln nLn);
    bool operator ==(ln nLn);
    bool operator !=(ln nLn);
    bool operator >=(ln nLn);
    bool operator > (ln nLn);
    bool operator <=(ln nLn);
    bool operator < (ln nLn);
    
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
