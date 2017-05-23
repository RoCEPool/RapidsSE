/*============================================================================*/
/*  on.hpp - offset number                                                    */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class on
  {
    
  private:
    i8 m_uOffsetNo;

  public:
    on(void);
    //on(u8 uOffsetNo, float fValue);
    ~on(void);
    
    static on nZero(void);
    
    ln        nToLevelNo(void);
    
    void  operator =(i8 u);
    bool operator ==(i8 u);
    bool operator !=(i8 u);
    bool operator >=(i8 u);
    bool operator <=(i8 u);
    bool operator <(i8 n);
    u8 operator *(void);
    on operator +(i8 u);
    on operator -(i8 u);
    on operator ++(int);
    on operator --(int);
  };
  
  on operator "" _on(unsigned long long u);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
