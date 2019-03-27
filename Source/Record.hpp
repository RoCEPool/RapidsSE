/*============================================================================*/
/* Record.hpp -- Record                                                       */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_MAX_RECORD = C_16KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Record : public Object
  {
  private:
    s16KB m_sRecStr; // the record as a string
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Record(void);
    ~Record(void) noexcept(false);
  
    void vAppendBcd(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);
    void vAppendBol(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);
    void vAppendDte(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);
    void vAppendFlt(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);
    void vAppendInt(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);
    void vAppendIvl(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);
    void vAppendTme(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);
    void vAppendTsp(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);
    void vAppendVch(Column* pCol, Datum* pDatum, w8 wDatum, Stz* szDatum);

    Stz* szRecStr(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Record* _pRecord(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
