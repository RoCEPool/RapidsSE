/*============================================================================*/
/* PuGen.hpp -- Performance Unit Data Generator                               */
/*============================================================================*/
#pragma once

namespace STORE { class ToRow; }

namespace STORE
{
  class PuGen
  {
  public:
    static void vGen(i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef);
  
    static void vPu10k (ToRow* pToRow, i8& riNextKey);
    static void vPu100k(ToRow* pToRow, i8& riNextKey);
    static void vPu1m  (ToRow* pToRow, i8& riNextKey);
    static void vPu10m (ToRow* pToRow, i8& riNextKey);

  private:
  
    static void vGroups(RowBuffer* pRowBuffer);
    static void vOther(RowBuffer* pRowBuffer, i8 iKey);
    static void vPrePst(RowBuffer* pRowBuffer);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
