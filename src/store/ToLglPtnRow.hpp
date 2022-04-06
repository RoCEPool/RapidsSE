/*============================================================================*/
/* ToLglPtnRow.hpp -- Convert row buff to logicl partitioned row buff         */
/*============================================================================*/
#pragma once

#include "HashSet.hpp"

namespace STORE
{
  class ToLglPtnRow : public Object
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
    
    ToLglPtnRow(void);
   ~ToLglPtnRow(void) noexcept(false);

    static void vRepairHashSet(Object* pObject);

    void vConvert(Table* pTable, RowBuffer* pRowBuffer);
  
    RowBuffer* pCompleted(i8 i_Ptn);
    RowBuffer* pDangling(i8 i_Ptn);
  
  private:
    void vCopyRow(i8 i_Ptn, i8 iBkt, Row* aRowSrc);
    void vNewDstRow(i8 i_Ptn, i8 iBkt);

    Footer* aFooter(void);
  };

  ToLglPtnRow* _pToLglPtnRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
