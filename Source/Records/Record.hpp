/*============================================================================*/
/* Record.hpp -- Record                                                       */
/*============================================================================*/
#pragma once

namespace Records
{
  class Record : public SP_Record
  {
  private:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Record(void);
    Record(StreamDef* pStreamDef, i8 iSubSet);
   ~Record(void) noexcept(false);
  
    void vAppend(RowSingle* pRow);
  
    void vPrintRow(void);
    void vPrintPartialRow(void);
  
    void vRecStr(Stz* szRecStr);
    Stz* szRecStr(void);

  private:
    Footer* aFooter(void);
  };
  
  Record* _pRecord(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
