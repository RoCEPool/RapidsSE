/*============================================================================*/
/* Record.hpp - Record sprint helper                                       */
/*============================================================================*/
#pragma once

static const i8 C_LOAD_READCHUNK    = C_1M; // its 1M not 1MB (i.e. < 1MB)

namespace STORE
{
  class Record : public Object
  {
  public:
    RowPix*    m_pRowPix;
    char       m_caRecord[C_MAX_RECORD];
    char*      m_azCursor;    // append cursor for StrLib::vAppend()

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Record(RowPix* pRowPix);
   ~Record(void) noexcept(false);
  
    char* az(void);
    Stz* sz(void);
    Stz* sz(Row* pRow, bool bNewLine=false);
  
    void vPrintAsCsv(void);
    void vPrintAsRow(Stz* szLabel=nullptr);

    void vToCsvRecord(Row* pRow);
  
  private:
    void vAddRow(Row* aRow);
    void vAppendCsvVal(eDM eDatum, i8 iScale, i8 iColVal);

    Footer* aFooter(void);
  };

  Record* _pRecord(void* aAddr);


  typedef HV<Record,RowPix>       HV_Record;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
