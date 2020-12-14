/*============================================================================*/
/* Record.hpp - Record sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Record : public Object
  {
  public:
    char       m_caRecord[C_MAX_RECORD];
  
    char*      m_azCursor;    // append cursor for StrLib::vAppend()
    StreamDef* m_pStreamDef;  // stream def
    i8         m_iSubSet;     // stream def subset

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Record(void);
    Record(StreamDef* pStreamDef, i8 iSubSet);
//    Record(RowSingle* pRowSingle);
   ~Record(void) noexcept(false);
  
  
    char* az(void);
    Stz* sz(void);
    Stz* sz(Row* pRow);
  
    void vToRecord(RowSingle* pRowSingle);
  
    void vPrintAsCsv(void);
    void vPrintAsRow(void);

  private:
    void vAddRow(Row* aRow);
    void vAddColVal(eDM eDatum, i8 iScale, i8 iColVal);

    void vReset(void);

    Footer* aFooter(void);
  };

  Record* _pRecord(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
