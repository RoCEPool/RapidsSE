/*============================================================================*/
/* ToLglPtnRow.hpp -- Convert row buff to logicl partitioned Pds row buff     */
/*============================================================================*/
#pragma once

#include "LglBufSet.hpp"

namespace STORE
{
  class ToLglPtnRow : public Object
  {
  private:
  
    LglBufSet  m_oLglBufSet;

    Pds* m_pIntoPdsRowBuffer;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToLglPtnRow(void);
   ~ToLglPtnRow(void) noexcept(false);

    static void vRepairHashSet(Object* pObject);

    void vConvert(Table* pTable, Pds* pPdsRowBuffer);

    Pds* pCompleted(LglPtn tLglPtn);
    Pds* pDangling(LglPtn tLglPtn);
  
  private:
    void vCopyRow(LglPtn tLglPtn, Row* aRowSrc, RowPix* pFromRowPix);

    Footer* aFooter(void);
  };

  ToLglPtnRow* _pToLglPtnRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
