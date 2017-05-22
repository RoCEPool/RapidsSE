/*============================================================================*/
/*  nn.hpp - offset number                                                    */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class on
  {
    
  private:
    u8 m_uOffsetNo;

  public:
    on(void);
    on(u8 uOffsetNo);
    ~on(void);
    
    static on nNil(void);
    ln        nToLevelNo(void);
    
    bool operator ==(on n);
    bool operator !=(on n);
    bool operator >=(on n);
    bool operator <=(on n);
    u8 operator *(void);
    on operator +(u8 u);
    on operator -(u8 u);
    on operator ++(int);
  };
  
  on operator "" _on(unsigned long long u);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
