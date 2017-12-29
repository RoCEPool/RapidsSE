/*============================================================================*/
/* PerformanceEntry.hpp -- Performance Entry                                  */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class PerformanceEntry : public Entry
  {
    private:
  
    i8  m_iTicks; // tick count for this entry
    s32 m_sNote;  // note for this entry
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void* operator new(size_t, PerformanceEntry*);
    void  operator delete(void*);
    
    PerformanceEntry(void);
    ~PerformanceEntry(void);
  
    void vSet(i8 iTicks, Stz* aNote);
  
    i8 iTicks(void);
    Stz* aNote(void);
  
    Footer* aFooter(void);
  };
  
  PerformanceEntry* _pPerformanceEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
