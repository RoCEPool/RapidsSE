/*============================================================================*/
/* RN_Int.hpp - Int random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Int : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Int(void);
    RN_Int(Datum* pDatum, Column* pCol);
   ~RN_Int(void) noexcept(false);
  
    void vRndInt(Datum* pDatum, Column* pCol);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
