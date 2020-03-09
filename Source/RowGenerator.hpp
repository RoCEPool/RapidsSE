/*============================================================================*/
/* RowGenerator.hpp -- Row Generator                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowGenerator
  {
  public:
    static void vRandom(RowSingle* pRow, StreamDef* pStreamDef);
  
  private:
    static void vRndBcd(RowSingle* pRow, Column* pCol);
    static void vRndBol(RowSingle* pRow, Column* pCol);
    static void vRndDte(RowSingle* pRow, Column* pCol);
    static void vRndFlt(RowSingle* pRow, Column* pCol);
    static void vRndInt(RowSingle* pRow, Column* pCol);
    static void vRndIvl(RowSingle* pRow, Column* pCol);
    static void vRndTme(RowSingle* pRow, Column* pCol);
    static void vRndTsp(RowSingle* pRow, Column* pCol);
    static void vRndVch(RowSingle* pRow, Column* pCol);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
