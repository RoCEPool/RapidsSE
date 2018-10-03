/*============================================================================*/
/* SubLoader.hpp -- Subordinate Parallel Table LoadTable                      */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class SubLoader : public Object
  {
  private:
    Job_SubLoader* m_pJob_SubLoader; // job
  

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, SubLoader* pSubLoader);
    void  operator delete(void* pLoaderAddress);
 
    SubLoader(Job_SubLoader* pJob_SubLoader);
    ~SubLoader(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vBegin(void);
    void vEnd(void);
    bool bFetchRecord(void);
    void vHashRecord(void);
    bool bInsertRow(void);
    Job_SubLoader* pJob_SubLoader(void);
    bool bProcessRecords(void);
    bool bReadChunk(void);
  
    Footer* aFooter(void);
  };
  
  SubLoader* _pLoaderSub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
