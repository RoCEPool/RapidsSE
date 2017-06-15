/*============================================================================*/
/* CandidatePaths.hpp - Set of Candidate Paths                                */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class CandidatePaths : public Object
  {
    private:
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    CandidatePaths(void);
    ~CandidatePaths(void);
  
    void vGenPaths(Table* pTable, Index* pIndex);
    void vGenPaths(Table* pTable);
  
    void vShowPaths(void);
  
    Footer* pFooter(void);
  };
  
  CandidatePaths* _pCandidatePaths(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
