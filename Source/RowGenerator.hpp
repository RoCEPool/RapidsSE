/*============================================================================*/
/* RowGenerator.hpp -- Row Generator                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowGenerator
  {
  public:
    static void vRandom(Row* pRow, StreamDef* pStreamDef);
  
  private:
    static void vRndBcd(Row* pRow, Column* pCol);
    static void vRndBol(Row* pRow, Column* pCol);
    static void vRndDte(Row* pRow, Column* pCol);
    static void vRndFlt(Row* pRow, Column* pCol);
    static void vRndInt(Row* pRow, Column* pCol);
    static void vRndIvl(Row* pRow, Column* pCol);
    static void vRndTme(Row* pRow, Column* pCol);
    static void vRndTsp(Row* pRow, Column* pCol);
    static void vRndVch(Row* pRow, Column* pCol);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
