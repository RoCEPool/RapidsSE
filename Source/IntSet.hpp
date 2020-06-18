/*============================================================================*/
/* IntSet.hpp -- Int Set                                                      */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class IntSet : public Object
  {
  private:
  
    ArrayNL_t<CN_INTSET,i8,C_64KB>*  m_pIntAry;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IntSet(void);
    IntSet(i8 nInts);
   ~IntSet(void) noexcept(false);
  
    void vAscending(void);
    void vDescending(void);
    void vRandomise(void);

    i8 iGet(i8 i_Item);
    i8 nInts(void);
    void vSet(i8 i_Item, i8 iInt);

    Footer* aFooter(void);
  };
  
  IntSet* _pIntSet(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
