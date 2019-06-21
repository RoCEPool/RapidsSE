/*============================================================================*/
/* Datum.hpp -- Datum (column value outside of row)                           */
/*============================================================================*/
#pragma once

namespace Data
{
  class Datum : public Object
  {
  public:
  
    Column* m_pCol;   // which column
    eDM     m_eDatum; // the kind of datum
    Dtm     m_wDtm;   // the value
  
    // - - - - - - -
    i8      m_nChars; // number of chars
    sVCH    m_sVch;   // vch value

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Datum(void);
    ~Datum(void) noexcept(false);

    bool bIsNull(void);
    void vSetNull(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Datum* _pDatum(void* aAddr);
}


#pragma pack(push, 1)
class Nibbler1 {
  struct {
    u8 m_uNibble : 4 ; //
  } m_xaNibbles[128];
};
class Nibbler2 {
  struct {
    u8 m_uNibbleL : 4 ; //
    u8 m_uNibbleR : 4 ; //
  } m_xaNibbles[64];
};
#pragma pack(pop)


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
