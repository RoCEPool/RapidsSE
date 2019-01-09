/*============================================================================*/
/* PerformanceEntry.hpp -- Performance Entry                                  */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class PerformanceEntry : public Item
  {
    private:
  
    i8  m_iTicks; // tick count for this entry
    s32 m_sWord;  // name of the word the pp: is in
    s32 m_sNote;  // note for this entry
  
    // must be last
    Footer m_xFooter;
    
    public:
        
    PerformanceEntry(void);
    ~PerformanceEntry(void) noexcept(false);
  
    void vSet(i8 iTicks, Stz* aWord, Stz* aNote);
  
    i8 iTicks(void);
    Stz* aWord(void);
    Stz* aNote(void);
  
    Footer* aFooter(void);
  };
  
  PerformanceEntry* _pPerformanceEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
