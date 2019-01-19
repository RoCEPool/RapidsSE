/*============================================================================*/
/* Statistics.hpp -- Statistics                                               */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class Statistics : public Object
  {
  public:
  
  private:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    Statistics(void);
    ~Statistics() noexcept(false);
  
  private:
  
    Footer* aFooter(void);
  };
  
  Statistics* _pStatistics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
