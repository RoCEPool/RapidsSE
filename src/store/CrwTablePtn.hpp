/*============================================================================*/
/* CrwTablePtn.hpp -- Crw Table Partition                                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  class CrwTablePtn : public Object
  {
  private:
    Table* m_pTable;        // table this partition belongs to
    Bucket*   m_pBucket;    // bucket this partition belongs to
    LglPtn    m_tLglPtn;        // partition number (in grid)

    CrwBuffer* m_pRootBuffer;    // the data root buffer
    CrwBuffer* m_pCurrentBuffer; // the data current buffer for insert

    // Note that the IndexTreeVec gets moved upto TablePtn
    //IndexTreeVec* m_pIndexTreeVec;  // vector of index trees
      
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CrwTablePtn(Table *pTable, LglPtn tLglPtn, Bucket *pBucket);
   ~CrwTablePtn(void) noexcept(false);
  

    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    i8   nBuffers(void);
    void vDump(void);




    bool bPopulateIndexTree(IndexTree* pIndexTree);

    CrwBuffer* pRootCrwBuffer(void);
    Table* pTable(void);
    void vTruncate(void);

    bool bVerifyTable(void);
    void vVisualizeTable(void);
    
  private:
    Footer* aFooter(void);
  };
  
  CrwTablePtn* _pCrwTablePtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
