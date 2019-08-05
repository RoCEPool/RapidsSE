/*============================================================================*/
/* LogEntry.hpp - Log Entry                                                   */
/*============================================================================*/
#pragma once

namespace Logging
{
  class LogEntry : public Object
  {
  friend class Log;

  private:
    i8   m_i_Entry;     // entry no
    i8   m_iTick;       // tick when entry added
    char m_caNode[32];  // which node
  
    eEK  m_eKind;       // log entry kind
  
    union {
      _LogEntryText m_xText;
    };
  
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
  
    void vWrite(File* pFile, bool bShortForm, i8 iTickDelta, i8& iTotalDelta);
    void vWriteShort(File* pFile);
    void vWriteShortForeign(File* pFile);

    Footer* aFooter(void);
  };
  
  LogEntry* _pLogEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
