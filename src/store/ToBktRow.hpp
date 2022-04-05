/*============================================================================*/
/* ToBktRow.hpp -- Convert row buffers to partitioned row slab buffers        */
/*============================================================================*/
#pragma once

#include "HashSet.hpp"

namespace STORE
{
  class ToBktRow : public Object
  {
  private:
  
    HashSet  m_oHashSet;
  
    RowBuffer* m_pFromRowBuffer; // the

    RowBuffer* m_pIntoRowBuffer;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToBktRow(void);
   ~ToBktRow(void) noexcept(false);

    static void vRepairHashSet(Object* pObject);

    void vConvert(Table* pTable, RowBuffer* pRowBuffer);
  
    RowBuffer* pCompleted(i8 i_Ptn);
    RowBuffer* pDangling(i8 i_Ptn);
  
  private:
    void vCopyRow(i8 i_Ptn, Row* aRowSrc);
    void vNewDstRow(i8 i_Ptn);

    void vCopyRow(i8 i_Ptn, i8 iBkt, Row* aRowSrc);
    void vNewDstRow(i8 i_Ptn, i8 iBkt);

    Footer* aFooter(void);
  };

  ToBktRow* _pToBktRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
