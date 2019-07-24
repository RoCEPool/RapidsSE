/*============================================================================*/
/* LogEntry.hpp - Log Entry                                                   */
/*============================================================================*/
#pragma once

namespace Logging
{
  static const i8 C_LOG_TEXT = 128;
  
  class LogEntry : public Object
  {
  friend class Log;
  friend class Log;

  private:
    i8   m_i_Entry;     // entry no
    i8   m_iTick;       // tick when entry added
    char m_caNode[32];  // which node
  
    i8   m_iEnumGroup;  // enum group
    i8   m_iEnum;       // enum/jobid
    i8   m_iForeign;    // foreign job
  
    char m_caText[C_LOG_TEXT]; // comment
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    LogEntry(void);
   ~LogEntry(void) noexcept(false);
  
    void vInit(void);
    void vSet(i8 iEnumGroup, i8 iEnum, i8 iForeign, char* aText);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - -
  
    void vDump(File* pFile, bool bShortForm,
               LogEntry* pPrevMark, i8& iTotalDelta);
  
    void vDumpShort(File* pFile);
    void vDumpShortForeign(File* pFile);

    Footer* aFooter(void);
  };
  
  LogEntry* _pLogEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
