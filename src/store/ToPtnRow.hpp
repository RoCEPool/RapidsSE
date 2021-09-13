/*============================================================================*/
/* ToPtnRow.hpp -- Convert row buffers to partitioned row slab buffers        */
/*============================================================================*/
#pragma once

#include "HashSet.hpp"

namespace STORE
{
  class ToPtnRow : public Object
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
    
    ToPtnRow(void);
   ~ToPtnRow(void) noexcept(false);

    static void vRepairHashSet(Object* pObject);

    void vConvert(RowBuffer* pRowBuffer);
  
    RowBuffer* pCompleted(i8 i_Ptn);
    RowBuffer* pDangling(i8 i_Ptn);
  
  private:
    void vCopyRow(i8 i_Ptn, Row* aRowSrc);
    void vNewDstRow(i8 i_Ptn);

    Footer* aFooter(void);
  };

  ToPtnRow* _pToPtnRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
