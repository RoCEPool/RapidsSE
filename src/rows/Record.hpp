/*============================================================================*/
/* Record.hpp - Record sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Record : public Object
  {
  public:
    StreamDef* m_pStreamDef;  // stream def
 
    RowPix     m_oRowPix;

    char       m_caRecord[C_MAX_RECORD];
    char*      m_azCursor;    // append cursor for StrLib::vAppend()

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Record(StreamDef* pStreamDef);
   ~Record(void) noexcept(false);
  
    char* az(void);
    Stz* sz(void);
    Stz* sz(Row* pRow);
  
    void vPrintAsCsv(void);
    void vPrintAsRow(void);

    void vToCsvRecord(Row* pRow);
  
  private:
    void vAddRow(Row* aRow);
    void vAppendCsvVal(eDM eDatum, i8 iScale, i8 iColVal);

    Footer* aFooter(void);
  };

  Record* _pRecord(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
