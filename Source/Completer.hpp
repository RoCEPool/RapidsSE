/*============================================================================*/
/* Completer.hpp -- multi-partition completer                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Completer : public Object
  {
  private:
    Lock m_okLock; // lock over the following members
  
    i8   m_nPartitionsComplete; // number of partitions indicated completion
    i8   m_nUnitsExpected;      // total number of work units expected
    i8   m_nUnitsProcessed;     // total number of work units processed

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Completer* pCompleter);
    void  operator delete(void* aAddress);
    
    Completer(void);
    ~Completer(void) noexcept(false);
  
    bool IncProcessed(i8 m_nUnitsExpected);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
