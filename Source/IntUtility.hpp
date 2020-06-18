/*============================================================================*/
/* IntUtility.hpp -- Int Utility                                              */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class IntUtility : public Object
  {
  private:
  
    ArrayNL_t<CN_INTUTILITY,i8,C_64KB>*  m_pIntAry;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IntUtility(void);
    IntUtility(i8 nInts);
   ~IntUtility(void) noexcept(false);
  
    void vAscending(void);
    void vDescending(void);

    i8 iGet(i8 i_Item);
    i8 nInts(void);
    void vSet(i8 i_Item, i8 iInt);

    Footer* aFooter(void);
  };
  
  IntUtility* _pIntUtility(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
