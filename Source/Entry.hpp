/*============================================================================*/
/* Entry.hpp -- index entry                                                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry
  {
  private:
    RowId  m_xRowId;  // rowid
    u8     m_nLevels; // no of levels
    WspVbl m_xWsp;    // workspace
  
  public:
    static i8 iBytesRequired(i8 iDataBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(i8 iDataBytes);
    Entry(i8 nDataBytes, i8 nLevels);
    Entry(EntWsp& xEntWsp);
    ~Entry(void);
    
    void  vContentDump(void);

    RowId& xRowId(void);
    void   vRowId(RowId& xRowId);

    void   vEntry(EntWsp& xEntWsp);

    Key&   xKey(void);
    Key*   aKey(void);
    void   vKey(Key& xKey);
    void   vKey(KeyWsp& xKeyWsp);
  
    u8     nLevels(void);
    void   vLevels(u8 nLevels);
    
    u8     nOffset(u8 nLevels);
    void   vOffset(u8 nLevels, u8 nOffset);

  
    //------
  
    i8    nSize(void);
  
    RightVec* aRightVec(void);
    Entry* aRightEntry(i8 iLevel);
  
    Footer* pFooter(void);
  };
  
  Entry* _aEntry(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
