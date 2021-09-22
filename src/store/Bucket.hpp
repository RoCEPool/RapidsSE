/*============================================================================*/
/* Bucket.hpp -- Bucket contains row buffer                                   */
/*============================================================================*/
#pragma once

namespace STORE { class CrwTablePtn; class RowTablePtn; }

namespace STORE
{
  class Bucket : public Object
  {
  private:
    //Lock   m_okLock;  // lock on the partition
    Table* m_pTable;  // table this partition belongs to
#ifdef WITH_PTNID
    PtnId m_tPtnNo;  // partition number (in grid)
#else
    tPN    m_tPtnNo;  // partition number (in grid)
#endif

    eCN    m_eKind;   // what kind of table partition

    union {
      CrwTablePtn* m_pCrwTablePtn;
      RowTablePtn* m_pRowTablePtn;
    };

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
#ifdef WITH_PTNID
    Bucket(Table* pTable, PtnId tPtnNo, eCN eKind);
#else
    Bucket(Table* pTable, tPN tPtnNo, eCN eKind);
#endif
    ~Bucket(void) noexcept(false);

    static void vRepair(Object* pObject);
#ifdef WITH_PTNID
    PtnId  tPtnNo(void);
#else
    tPN  tPtnNo(void);
#endif
    Table* pTable(void);

    void vAddIndex(IndexTree* pIndexTree);
    void vAppendBuffer(DataBuffer* pDataBuffer);
    i8   nBuffers(void);
    void vDump(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    void vInsertIndexEntries(CrwBuffer* pCrwBuffer);
    bool bPopulateIndex(i8 iIndexNo);
    bool bPopulateIndexTree(IndexTree *pIndexTree);
    void vRemoveIndex(i8 iIndexNo);
    void vAnalyze(i8& nBuffers, i8& nRows);
    CrwBuffer* pRootCrwBuffer(void);
    RowBuffer* pRootRowBuffer(void);
    DataBuffer* pRootBuffer(void);
    void vTruncate(void);
    void vTruncateIndex(Index* pIndex);
    bool bVerifyTable(void);
    void vVisualizeTable(void);

  private:
    Footer* aFooter(void);
  };

  Bucket* _pBucket(void* aAddr);


  typedef VectorNL_t<CN_BUCKETVEC,STORE::Bucket,C_MAX_PTNSPERNODE>BucketVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

