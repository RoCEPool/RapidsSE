/*============================================================================*/
/* UT_SkipList.hpp - SkipList Unit Test                                       */
/*============================================================================*/
#pragma once

#include "UT.hpp"
#include "UnitInt.hpp"

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
    static bool bMoveRight_int(i8 iVal, UnitInt* pUnitInt, bool& bFoundKey);
    static void vPrintUnitInt(SkipNode* pNode, int i_Level);

    void vInsertUnitInts(void);
    void vRemoveUnitInts(void);
  
    void vPrintSkipList(void);
    void vValidateSkipList(void);


    Footer* aFooter(void);
  };

  UT_SkipList* _pUT_SkipList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
