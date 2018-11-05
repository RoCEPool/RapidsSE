/*============================================================================*/
/* ActivityMark.hpp - Activity Mark                                           */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class ActivityMark : public Object
  {
  friend class ActivityMap;
  
  private:
    i8   m_iMark;      // mark no
    i8   m_iTick;      // tick of mark
    char m_aNode[32];  // which node
    i8   m_iJobId;     // which job
    char m_aMark[128]; // comment
  
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
    void vDump(File* pFile, i8 i_Mark, ActivityMark* pPrevMark, i8& iTotalDelta);
  
    Footer* aFooter(void);
  };
  
  ActivityMark* _pActivityMark(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
