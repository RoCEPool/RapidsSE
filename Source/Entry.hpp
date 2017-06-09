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
    // ..data..        // right offsets & key
    // ..footer..
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(EntWsp& xEntWsp);
    Entry(i8 nBytes, i8 uOffsets);
    ~Entry(void);
    
    void   vShow(i8 iOffset, IdxColVec* pColVec);

    IndexBuffer* pIndexBuffer(void);
    Id&    xId(void);
    void   vId(Id xId);
    void   vKey(EntWsp& xEntWsp);
    void   vSetEntry(EntWsp& xEntWsp);
    i8     nSizeEntry(void);
  
  private:
    bn     nBufferNo(void);
  };
  
  Entry* _pEntry(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
