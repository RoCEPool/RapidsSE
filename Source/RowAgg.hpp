/*============================================================================*/
/* RowAgg.hpp -- Row Aggregation                                              */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class RowAgg : public Object
  {
  static const i8 C_MAX_COLAGG = 16;

  private:
    Table*     m_pTable;     // which table
    Column*    m_pKeyColumn; // the key column
    ColumnVec* m_pAggColVec; // columns being aggregated
  
    Since m_oSince;  // since of buffer
    i8    m_nRows;
  
    i8    m_iaMinKey[C_MAX_COLAGG];
    i8    m_iaMinVal[C_MAX_COLAGG];
  
    i8    m_iaMaxKey[C_MAX_COLAGG];
    i8    m_iaMaxVal[C_MAX_COLAGG];
  
    i8    m_iaSumVal[C_MAX_COLAGG];
    i8    m_iaAvgVal[C_MAX_COLAGG];

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RowAgg(void);
    ~RowAgg(void) noexcept(false);
  
    void vAdd(RowAgg* pAggArg);
    void vAggregate(i8 i_Slot, CrwBuffer* pCrwBuffer);
    void vPrintOut(File& oOutputFile);
    void vReadIn(File& oInFile);
    void vReset(void);
    void vTable(Table* pTable);
    void vWriteOut(File& oOutputFile);

  private:
    Footer* aFooter(void);
  
    void vDiscoverColumns(void);
  
  };
  
  RowAgg* _pRowAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
