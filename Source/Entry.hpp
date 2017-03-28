/*============================================================================*/
/* Entry.hpp -- index entry                                                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  /*
   
   important note: 
     iLevel is assumed to be 0..C_FLAT_LEVELS-1
     iLevelSub is iLevel - 1 (ie 0..C_FLAT_LEVELS-2)
   
     we never use iLevel - 1 when we mean iLevelSub
     we always compute iLevelSub first
   
   */
  
  class Entry //: public Object
  {
  private:
    i8     m_nBytes;  // overall size of this entry
    RowId  m_xRowId;  // rowid
    u8     m_nLevels; // no of levels
    WspVbl m_xWsp;    // workspace
    // ..data..
  
  public:
    static i8 nBytesRequired(i8 nLevels, i8 nKeyBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(i8 nDataBytes, i8 nLevels);
    ~Entry(void);
    
    void   vContentDump(void);
    Key*   aKey(void);
    u8     nLevels(void);
    u8     uOffset(u8 iLevel);
    void   vOffset(u8 iLevel, u8 uOffset);
    Entry* aRightEntry(i8 iLevel);
    RowId& xRowId(void);
    void   vSetEntry(EntWsp& xEntWsp);
    i8     nSize(void);

    Footer* pFooter(void);
  };
  
  Entry* _aEntry(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
