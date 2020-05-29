/*============================================================================*/
/* on.hpp - offset number                                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class on
  {
    
  private:
    i8 m_iOffsetNo;

  public:
    on(void);
    explicit on(i8 i);
    ~on(void);
    
    ln        nToLevelNo(void);
  
  operator i8(); // auto conversion to i8
  
    void operator =(i8 i);
    bool operator ==(i8 i);
    bool operator !=(i8 i);
    bool operator >=(i8 i);
    bool operator > (i8 i);
    bool operator <=(i8 i);
    bool operator < (i8 i);
  
    void operator =(on nOn);
    bool operator ==(on nOn);
    bool operator !=(on nOn);
    bool operator >=(on nOn);
    bool operator > (on nOn);
    bool operator <=(on nOn);
    bool operator < (on nOn);
 
    i8 operator *(void);
    on operator +(i8 i);
    on operator -(i8 i);
    on operator ++(int);
    on operator --(int);
  
    bool bInRange(void);
  };
  
  on operator "" _on(unsigned long long u);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
