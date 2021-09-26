/*============================================================================*/
/* UT_SkipList.hpp - SkipList Unit Test                                       */
/*============================================================================*/
#pragma once

#include "UT.hpp"

namespace BASE
{
  class UT_SkipList : public UT
  {
  public:

    IntSet*     m_pIntSet;
  
    SkipListNL* m_pSkipList;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    UT_SkipList(void);
   ~UT_SkipList(void) noexcept(false);
 
    static bool bTest(void);

  public:
    bool bRunTests(void);

  private:
    Footer* aFooter(void);
  };

  UT_SkipList* _pUT_SkipList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
