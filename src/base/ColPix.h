/*============================================================================*/
/* ColPix.h - Column Picture                                                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  // the ColPix structure is 64b / u8

  union ColPix // condensed column def 
  {
    struct {
      u8 m_uNumber    :  8; // number
      u8 m_uName      : 16; // name offset
      u8 m_uKind      :  3; // cell contains (user col, ptn hash, ...) eCX
      u8 m_uType      :  4; // up to 16 column types eCT
      u8 m_uDatum     :  4; // up to 16 datums eDM
      u8 m_uLength    : 16; // up to 65,536
      u8 m_uScale     :  5; // up to 31 places
      u8 m_bNullable  :  1; // is allowed to be null?
      u8 m_bPtnKey    :  1; // is a partitioning column?
      u8 m_uSpare     :  6; // round up to 64b
    //                 ---
    //                 64b  // so... 256 columns is 256 * 8B = 2KB
    //                 ===
    };
    
    u8 m_uAsU8;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
