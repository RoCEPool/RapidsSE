/*============================================================================*/
/* Totup.hpp - Totup                                                          */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_MAX_TOT_ITEMS = 32;

  class Totup : public Object
  {
  public:
  
    struct
    {
      i8 m_nCnt;
      i8 m_iSum;
      i8 m_iMin;
      i8 m_iMax;
    }
    m_xaItem[C_MAX_TOT_ITEMS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Totup(void);
   ~Totup(void) noexcept(false);
      
    void vAdd(i8 i_Item, i8 iQty);
    i8   nCnt(i8 i_Item);
    i8   iMax(i8 i_Item);
    i8   iMin(i8 i_Item);
    i8   iSum(i8 i_Item);
    void vReset(void);
  
    Footer* aFooter(void);
  };

  Totup* _pTotup(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
