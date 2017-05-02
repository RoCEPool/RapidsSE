/*============================================================================*/
/* Entry.hpp -- index entry (sequential variant)                              */
/*============================================================================*/
#pragma once

namespace DBobjects
{  
  class EntrySeq //: public Object
  {
  private:
    i8     m_nBytes;  // overall size of this entry
    Id     m_xId;     // id
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
    Id&    xId(void);
    void   vSetEntry(EntWspSeq& xEntWspSeq);
    i8     nSize(void);

    Footer* pFooter(void);
  };
  
  EntrySeq* _aEntrySeq(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
