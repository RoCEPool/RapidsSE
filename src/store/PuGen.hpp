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
  
    static void vGen10k (i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef);
    static void vGen100k(i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef);
    static void vGen1m  (i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef);
    static void vGen10m (i8& riNextKey, ToRow* pToRow, StreamDef* pStreamDef);

  private:
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
