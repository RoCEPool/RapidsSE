/*============================================================================*/
/* EntWrk.hpp -- Working Entry (contains an embedded object)                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWrk : public Object
  {
  friend class Entry;
  friend class Table;
  
  static const i8 C_DATA_BYTES = C_1KB*15;
  
  private:
    Id m_xId;      // id of row, or referenced entry in another buffer
    i8 m_iTxId;    // transaction id
    i8 m_nOffsets; // number of offsets (0..3)
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key and datums
    WspFxd<C_DATA_BYTES> m_xEmbeddedWsp; // embedded object workspace
  
    Footer m_xFooter;
  
  public:
  EntWrk(eCN eClass);
    ~EntWrk(void);
  
    char*  aEmbedded(void);
    Id&    xId(void);
    void   vId(Id& xId);
    char*  aLatter(void);
    i8     nOffsets(void);
    i8     nOffsetSize(void);
    void   vOffsets(i8 nOffsets);
    void   vTake(i8 iSize);
    i8     iTxId(void);
    void   vTxId(i8 iTxId);

    Footer* aFooter(void);
  
  protected:
    i8     nEmbeddedSize(void);
    void   vEmbeddedSize(i8 iSize);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
