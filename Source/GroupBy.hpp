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
    ~GroupBy(void);
  
    void vApplyBuffer(EntryBuffer* pEntryBuffer);
  
    Footer* aFooter(void);
  };
  
  GroupBy* _pGroupBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
