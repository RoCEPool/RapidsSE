/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once


#include "Bucket.hpp"

namespace STORE { class CrwTablePtn; class RowTablePtn; class Bucket;}

namespace STORE
{
  class TablePtn : public Object
  {
  private:
    Lock   m_okLock;  // lock on the partition
    eCN    m_eKind;   // what kind of table partition

    BucketVec*    m_pBucketVec;     // vector of buckets
    IndexTreeVec* m_pIndexTreeVec;  // vector of index trees

    Table* m_pTable;  // table this partition belongs to


    LglPtn m_tLglPtn;  // partition number (in grid)
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TablePtn(Table* pTable, LglPtn tLglPtn);
   ~TablePtn(void) noexcept(false);
  
    void vDropRead(void);
    void vDropWrite();
    bool bGrabRead(void);
    bool bGrabWrite(void);
    bool bIsLocked(void);
    eCN  eKind(void);

    LglPtn  tLglPtn(void);

    Table* pTable(void);

    Bucket* pGetBucket(i8 i_Bucket);
    void vSetBucket(i8 iBucket);

    void vAddBucket(Table* pTable, LglPtn tLglPtn, eCN eKind);

    void vRemoveBucket(i8 i_Bucket);
    void vAddIndex(IndexTree* pIndexTree);
    void vAppendBuffer(DataBuffer* pDataBuffer);
    void vAppendBuffer_replicated(DataBuffer* pDataBuffer);
    i8   nBuckets(void);
    i8   nBuffers(void);
    void vDump(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    void vInsertIndexEntries(CrwBuffer* pCrwBuffer);
    bool bPopulateIndex(i8 iIndexNo);
    void vRemoveIndex(i8 iIndexNo);
    void vAnalyze(i8& nBuffers, i8& nRows);
    //CrwBuffer* pRootCrwBuffer(void);
    //RowBuffer* pRootRowBuffer(void);
    //DataBuffer* pRootBuffer(void);
    void vTruncate(void);
    void vTruncateIndex(Index* pIndex);
    bool bVerifyTable(void);
    void vVisualizeTable(void);
    Stz* szDumpContents(void);
    
  private:
    Footer* aFooter(void);
  };
  
  TablePtn* _pTablePtn(void* aAddr);


  typedef VectorNL_t<CN_TABLEPTNVEC,STORE::TablePtn,C_MAX_PTNSPERNODE>TablePtnVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
