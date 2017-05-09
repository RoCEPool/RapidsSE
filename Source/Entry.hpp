/*============================================================================*/
/* Entry.hpp -- index entry (skip list variant)                               */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class IndexBuffer;
  
  class Entry : public Object
  {
  private:
    i8     m_nBytes;   // overall size of this entry incl Object & Footer
    Id     m_xId;      // id
    u8     m_nOffsets; // no of offsets for this entry (0 or 1)
    // ..data..        // right offsets & key
  
    // ..footer..
  
  public:
    static i8 nBytesRequired(EntWsp& xEntWsp);
    static i8 nBytesRequired(i8 nOffsets, i8 nKeyBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(EntWsp& xEntWsp);
    Entry(i8 nBytes, i8 nOffsets);
    ~Entry(void);
    
    void   vShow(i8 iOffset, IdxColVec* pColVec);
  
    EntryData* aEntryData(void);
    
    IndexBuffer* pIndexBuffer(void);
    Key*   aKey(void);
    bool   bMatches(Key* pKey, IdxColVec* pColVec, RelOpAry *pRelOpAry);
    u8     nOffsets(void);
    u8     uRightOffset(u8 iLevel);
    void   vRightOffset(u8 iLevel, u8 uOffset);
    RightVec* aRightVec(void);
    Id&    xId(void);
    void   vSetEntry(EntWsp& xEntWsp);
    void   vSetKey(EntWsp& xEntWsp);
    i8     nSize(void);

    Footer* pFooter(void);
    
  private:
    bn     nBufferNo(void);
  };
  
  Entry* _aEntry(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
