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
    i8   m_iMark;       // mark no
    i8   m_iTick;       // tick of mark
    char m_caNode[32];  // which node
    i8   m_iCategory;   // category
    i8   m_iJobId;      // which job
    char m_caMark[128]; // comment
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    ActivityMark(void);
    ~ActivityMark(void) noexcept(false);
  
    static i8 iAsMicroseconds(i8 iTickDelta);
  
    void vInit(void);
    void vWrite(i8 iJobId, char* aMark);
    void vDump(File* pFile, bool bShortForm,
               ActivityMark* pPrevMark, i8& iTotalDelta);
  
    void vDumpShort(File* pFile);

    Footer* aFooter(void);
  };
  
  ActivityMark* _pActivityMark(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
