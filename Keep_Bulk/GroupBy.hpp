/*============================================================================*/
/* GroupBy.hpp -- Group By                                                    */
/*============================================================================*/
#pragma once

namespace MockEE
{
  class GroupBy : public Object
  {
  public:
  
    Footer m_xFooter;
    
  public:
    AccumVec* m_pAccumVec;
  
    void* operator new(size_t);
    void  operator delete(void*);
    
    GroupBy(void);
    ~GroupBy(void) noexcept(false);
  
    void vApplyBuffer(EntryBuffer* pEntryBuffer);
    void vApplyEntry(Entry* pLineItemEntry);
    void vApplyIndex(Entry* pIndexEntry);
    void vApplyPartial(GroupBy* pPartialGoupBy);
    void vEnd(void);
    void vStart(void);
  
    Footer* aFooter(void);
  
    static Entry* pToLineItemEntry(Index* pIndex, Entry* pEntry);
  };
  
  GroupBy* _pGroupBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
