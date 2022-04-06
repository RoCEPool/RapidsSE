/*============================================================================*/
/* PuGen.hpp -- Performance Unit Data Generator                               */
/*============================================================================*/
#pragma once

namespace STORE { class ToKeyHshRow; }

namespace STORE
{
  class PuGen
  {
  public:
    static void vGen(i8& riNextKey, ToKeyHshRow* pToRow, StreamDef* pStreamDef);
  
    static void vPu10k (ToKeyHshRow* pToRow, i8& riNextKey);
    static void vPu100k(ToKeyHshRow* pToRow, i8& riNextKey);
    static void vPu1m  (ToKeyHshRow* pToRow, i8& riNextKey);
    static void vPu10m (ToKeyHshRow* pToRow, i8& riNextKey);

  private:
  
    static void vGroups(RowBuffer* pRowBuffer);
    static void vOther(RowBuffer* pRowBuffer, i8 iKey);
    static void vPrePst(RowBuffer* pRowBuffer);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
