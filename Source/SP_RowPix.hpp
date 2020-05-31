/*============================================================================*/
/* SP_RowPix.hpp - RowPix sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Records
{
  class SP_RowPix : public Sprint
  {
  public:
    char*    m_azCursor;    // append cursor for StrLib::vAppend()
    ColPix*  m_axaColPix;
    char     m_caRecord[C_MAX_RECORD];

    // must be last
    Footer m_xFooter;
    
  public:
    SP_RowPix(ColPix* axaColPix);
   ~SP_RowPix(void) noexcept(false);
    
    char* azRecord(void);
    void vReset(void);
    Stz* sz(i8* aiaCells);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
