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
    i8 m_nOffsets; // number of offsets (0, 1 or 2)
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key and datums
    WspFxd<C_DATA_BYTES> m_xDataWrk; // datum workspace
  
    Footer m_xFooter;
  
  public:
  EntWrk(eCN eClass);
    ~EntWrk(void);
  
    //void   vAddKey(KeyWrk& xKeyWrk);
    //char*  aData(void);
    char*  aData(void);
    i8     nDataSize(void);
    void   vDataSize(i8 iSize);
    void   vData(Stz* aData, i8 iSize);
    //void   vForm(Id& xId, KeyWrk& xKeyWrk);
    //bool   bHasData(void);
    //void   vHasData(bool bHasData);
    Id&    xId(void);
    void   vId(Id& xId);
    Key*   aKey(void);
    i8     nKeySize(void);
    char*  aLatter(void);
    i8     nOffsets(void);
    i8     nOffsetSize(void);
    void   vOffsets(i8 nOffsets);
    //Row*   aRow(void);
    void   vShow(ColumnVec* pColVec);
    void   vTake(i8 iSize);
    i8     iTxId(void);
    void   vTxId(i8 iTxId);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
