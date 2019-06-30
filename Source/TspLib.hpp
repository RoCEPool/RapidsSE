/*============================================================================*/
/* Time_se.h - system time                                                    */
/*============================================================================*/
#pragma once

namespace System
{
  class Since;
  
  class Time : public Object
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    Time(void);
    ~Time(void) noexcept(false);
    
    static Dtm wGetLocalNow(void);
    static Dtm wGetUTCNow(void);
  
    static Dtm wToLocal(Since* pSince);
    static Dtm wToUTC(Since* pSince);

    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
