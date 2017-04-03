/*============================================================================*/
/* Entry.hpp -- index entry (skip list variant)                               */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry : public Object
  {
  private:
    i8     m_nBytes;  // overall size of this entry incl Object & Footer
    RowId  m_xRowId;  // rowid
    u8     m_nLevels; // no of levels for this entry (0, 1 or 2)
    // ..data..       // right offsets & key
  
    // ..footer..
  
  public:
    static i8 nBytesRequired(EntWsp& xEntWsp);
    static i8 nBytesRequired(i8 nLevels, i8 nKeyBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(EntWsp& xEntWsp);
    Entry(i8 nBytes, i8 nLevels);
    ~Entry(void);
    
    void   vShow(void);
  
    EntryData* aEntryData(void);
    Key*   aKey(void);
    u8     nLevels(void);
    u8     uRight(u8 iLevel);
    void   vRight(u8 iLevel, u8 uRight);
    RightVec* aRightVec(void);
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
