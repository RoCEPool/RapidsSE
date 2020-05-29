/*============================================================================*/
/* CandidatePath.hpp - Set of Candidate Paths                                 */
/*============================================================================*/
#pragma once

namespace Plans
{
  class CandidatePath : public Object,
                        public ListNode_t<CandidatePaths>,
                        public List_t<Column>
  {
    private:
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    CandidatePath(void);
    ~CandidatePath(void);
  
    void vGenPaths(void);
  
    Footer* pFooter(void);
  };
  
  CandidatePath* _pCandidatePath(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
