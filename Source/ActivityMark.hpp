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
    char m_aNode[32];
    i8   m_iJobId;
    char m_aMark[128];
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    ActivityMark(void);
    ~ActivityMark(void);
  
    static i8 iAsMicroseconds(i8 iTickDelta);
  
    void vInit(void);
    void vWrite(i8 iJobId, char* aMark);
    void vDump(File* pFile, i8 i_Mark, ActivityMark* pPrevMark) ;
  
    Footer* aFooter(void);
  };
  
  ActivityMark* _pActivityMark(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
