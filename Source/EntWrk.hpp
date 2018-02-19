/*============================================================================*/
/* EntWrk.hpp -- Working Entry                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  class Key;
  
  class EntWrk : public Object
  {
  static const i8 C_DATA_BYTES = C_1KB*15;
  
  private:
    Id m_xId;      // id
    i8 m_iTxId;    // transaction id
    i8 m_nOffsets; // number of offsets (0..3)
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key and datums
    WspFxd<C_DATA_BYTES> m_xDataWrk; // data area workspace
  
    Footer m_xFooter;
  
  public:
  EntWrk(eCN eClass);
    ~EntWrk(void);
  
    char*  aData(void);
    i8     nDataSize(void);
    void   vDataSize(i8 iSize);
    void   vData(Stz* aData, i8 iSize);
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
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
