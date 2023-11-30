/*============================================================================*/
/* ToLglPtnRow.hpp -- Convert row buff to logicl partitioned row buff         */
/*============================================================================*/
#pragma once

#include "LglBufSet.hpp"

namespace STORE
{
  class ToLglPtnRow : public Object
  {
  private:
  
    LglBufSet  m_oLglBufSet;

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
    void vConvertViaPdsRowBuffer(Table* pTable, Pds* pPdsRowBuffer);
  
    RowBuffer* pCompleted(LglPtn tLglPtn);
    RowBuffer* pDangling(LglPtn tLglPtn);
  
  private:
    void vCopyRow(LglPtn tLglPtn, Row* aRowSrc, RowPix* pRowPix);
    void vNewDstRow(LglPtn tLglPtn, RowPix* pFromRowPix);

    Footer* aFooter(void);
  };

  ToLglPtnRow* _pToLglPtnRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
