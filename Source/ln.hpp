/*============================================================================*/
/*  ln.hpp - level number                                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ln
  {
    
  private:
    u8 m_uLevelNo;

  public:
    ln(void);
    ln(u8 uLevelNo);
    ~ln(void);
    
    static ln nNil(void);
    //on        nToOffsetNo(void);
    
    bool operator ==(ln n);
    bool operator !=(ln n);
    bool operator >=(ln n);
    bool operator <=(ln n);
    u8 operator *(void);
    ln operator +(u8 u);
    ln operator -(u8 u);
    ln operator ++(int);
  };
  
  ln operator "" _ln(unsigned long long u);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
