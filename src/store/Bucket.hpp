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
    LglPtn m_tLglPtn;  // partition number (in grid)

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


    Bucket(Table* pTable, LglPtn tLglPtn, eCN eKind);


    ~Bucket(void) noexcept(false);

    static void vRepair(Object* pObject);

    LglPtn  tLglPtn(void);

    Table* pTable(void);


    void vAppendBuffer(DataBuffer* pDataBuffer);
    i8   nBuffers(void);
    void vDump(void);




    bool bPopulateIndexTree(IndexTree *pIndexTree);

    void vAnalyze(i8& nBuffers, i8& nRows);
    CrwBuffer* pRootCrwBuffer(void);
    RowBuffer* pRootRowBuffer(void);
    DataBuffer* pRootBuffer(void);
    void vTruncate(void);
    void vTruncateIndex(Index* pIndex);
    bool bVerifyTable(void);
    void vVisualizeTable(void);
    Stz* szDumpContents(i8 i_Bucket);

  private:
    Footer* aFooter(void);
  };

  Bucket* _pBucket(void* aAddr);


  typedef VectorNL_t<CN_BUCKETVEC,STORE::Bucket,C_MAX_PTNSPERNODE>BucketVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

