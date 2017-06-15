/*============================================================================*/
/* CandidatePaths.hpp - Set of Candidate Paths                                */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class CandidatePaths : public Object,
                         public List_t<CandidatePath>
  {
    private:
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    CandidatePaths(void);
    ~CandidatePaths(void);
  
    void vGenPaths(Table* pTable, Index* pIndex, Column* pColumn);
    void vGenPaths(Table* pTable, Index* pIndex);
    void vGenPaths(Table* pTable);
  
    Footer* pFooter(void);
  };
  
  CandidatePaths* _pCandidatePaths(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
