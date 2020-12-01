/*============================================================================*/
/* ColPix.h - Column Picture                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  // the ColPix structure is 32b / u4

  union ColPix  // condensed column def 
  {
    struct {
      u4 m_uDatum     :  4; // up to 16 types eDM
      u4 m_uLength    : 16; // up to 65,536
      u4 m_uScale     :  5; // up to 31 places
      u4 m_bNullable  :  1; // is allowed to be null?
      u4 m_bPtnKey    :  1; // is a partitioning column?
      u4 m_uKind      :  3; // what cell contains (user col, ptn hash, ...)
      
      u4 m_uSpare     :  2; // round up to 32b

    //                 ---
    //                 32b // so... 256 columns is 256 * 4B = 1KB
    //                 ===
    };
    
    u4 m_uAsI4;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
