/*============================================================================*/
/* Entry.hpp -- index entry (sequential variant)                              */
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
  
  class EntrySeq //: public Object
  {
  private:
    i8     m_nBytes;  // overall size of this entry
    RowId  m_xRowId;  // rowid
    u8     m_nLevels; // no of levels
    // ..data..       // rights & key
  
  public:
    static i8 nBytesRequired(EntWsp& xEntWsp);
    static i8 nBytesRequired(i8 nLevels, i8 nKeyBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, EntrySeq*);
    
    EntrySeq(i8 nBytes, i8 nLevels);
    ~EntrySeq(void);
    
    void   vContentDump(void);
    EntryData* aEntryData(void);
    Key*   aKey(void);
    u8     nLevels(void);
    u8     uRight(u8 iLevel);
    void   vRight(u8 iLevel, u8 uRight);
    EntrySeq* aRightEntry(i8 iLevel);
    RightVec* aRightVec(void);
    RowId& xRowId(void);
    void   vSetEntry(EntWspSeq& xEntWspSeq);
    i8     nSize(void);

    Footer* pFooter(void);
  };
  
  EntrySeq* _aEntrySeq(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
