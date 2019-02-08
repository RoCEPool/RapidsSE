/*============================================================================*/
/* ActivityMark.hpp - Activity Mark                                           */
/*============================================================================*/
#pragma once

namespace Measurement
{
  static const i8 C_MARK_TEXT = 128;
  class ActivityMark : public Object
  {
  friend class ActivityMap;
  
  private:
    i8   m_iMark;       // mark no
    i8   m_iTick;       // tick when mark made
    char m_caNode[32];  // which node
    i8   m_iEnumGroup;  // enum group
    i8   m_iEnum;       // enum/jobid
  
    char m_caText[C_MARK_TEXT]; // comment
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    ActivityMark(void);
    ~ActivityMark(void) noexcept(false);
  
    static i8 iAsMicroseconds(i8 iTickDelta);
  
    void vInit(void);
    void vWrite(i8 iEnumCategory, i8 iEnum, char* aMark);
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
