/*============================================================================*/
/* EntWrk.hpp -- Working Entry                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  
  class EntWrk_v2 : public Object
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
    EntWrk_v2(eCN eClass);
    EntWrk_v2(eCN eClass, i8 nOffsets);
    EntWrk_v2(eCN eClass, Entry* pEntry);
    //EntWrk_v2(Id& xId, i8 iTxId, KeyWrk& xKeyWrk);
    ~EntWrk_v2(void);
  
    //void   vAddKey(KeyWrk& xKeyWrk);
    //char*  aData(void);
    char*  aData_v2(void);
    i8     nDataSize(void);
    void   vDataSize(i8 iSize);
    void   vData(Stz* aData, i8 iSize);
    //void   vForm(Id& xId, KeyWrk& xKeyWrk);
    //bool   bHasData(void);
    //void   vHasData(bool bHasData);
    Id&    xId(void);
    void   vId(Id& xId);
    //Key*   aKey(void);
    //i8     nKeySize(void);
    i8     nOffsets(void);
    void   vOffsets(i8 nOffsets);
    //Row*   aRow(void);
    void   vShow(ColumnVec* pColVec);
    i8     iTxId(void);
    void   vTxId(i8 iTxId);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
