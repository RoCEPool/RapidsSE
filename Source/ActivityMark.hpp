/*============================================================================*/
/* ActivityMark.hpp - Activity Mark                                           */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class ActivityMark : public Object
  {
  private:
    i8   m_iTick;
    char m_aMark[128];
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    ActivityMark(void);
    ~ActivityMark(void);
  
    void vInit(void);
    void vWrite(char* aMark);

    Footer* aFooter(void);
  };
  
  ActivityMark* _pActivityMark(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
