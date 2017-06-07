/*============================================================================*/
/* Entry.hpp -- index entry (skip list variant)                               */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class IndexBuffer;
  
  class Entry : public BufferItem
  {
  private:
    i8     m_nBytes;   // overall size of this entry incl Object & Footer
    Id     m_xId;      // id
    i8     m_iOffsets; // no of offsets for this entry (0,1 or 2)
    // ..data..        // right offsets & key
  
    // ..footer..
  
  public:
    static i8 nBytesRequired(EntWsp& xEntWsp);
    static i8 nBytesRequired(i8 nOffsets, i8 nKeyBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(EntWsp& xEntWsp);
    Entry(i8 nBytes, i8 uOffsets);
    ~Entry(void);
    
    void   vShow(i8 iOffset, IdxColVec* pColVec);
  
    EntryData* aEntryData(void);
    
    on     nHighestOffset(void);
    IndexBuffer* pIndexBuffer(void);
    Key*   aKey(void);
    i8     nOffsets(void);
    i8     iRightOffset(on nOffsetNo);
    void   vRightOffset(on nOffsetNo, i8 iRightOffset);
    RightVec* aRightVec(void);
    Id&    xId(void);
    void   vId(Id xId);
    void   vKey(EntWsp& xEntWsp);
    void   vSetEntry(EntWsp& xEntWsp);
    i8     nSize(void);

    Footer* pFooter(void);
    
  private:
    bn     nBufferNo(void);
  };
  
  Entry* _pEntry(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
