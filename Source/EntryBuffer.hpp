/*============================================================================*/
/* EntryBuffer.hpp -- Entry Buffer                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  
  enum eBD // Buffer Durability
  {
  /* 0 */ BD_NONE=0,
  /* 1 */ BD_TEMPORARY,
  /* 2 */ BD_PERMANENT,
  /* 3 */ BD_ENUMS
  };
  
  static Str* aRD[]
  {
  /* 0 */ "NONE"_Str,
  /* 1 */ "TEMPORARY"_Str,
  /* 2 */ "PERMANENT"_Str,
  /* 3 */ "ENUMS"_Str,
  };
  
  static const i8 C_BUFFER_THRESHOLD = 100;
  
  static const RowBuffer* C_ROWBUFFER_MARKER = (RowBuffer*)0xFFFFFFFFFFFFFFFF;
  
  class EntryBuffer : public Buffer
  {
  protected:
    eBD   m_eDurability; // buffer durability
    i8    m_nEntries;    // number of entries in this buffer
  
    EntryBuffer* m_pPrevBuffer;  // prev buffer
    EntryBuffer* m_pNextBuffer;  // next buffer
    EntryBuffer* m_pHigherLevel; // higher level buffer that has entries
                                 // that point to this buffer
  
    Index*       m_pIndex;       // index this buffer belongs to
    i8           m_nBufferLevel; // index buffer level
  
    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB> m_xWsp;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer(void);
    EntryBuffer(i8 iDataBytes);
  
    EntryBuffer(eCN eClassNo);
    ~EntryBuffer(void);
  
    static EntryBuffer* pNewEntryBuffer(eCN eClassNo);
  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  
    i8   nEntries(void);
  
    EntryBuffer* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer* pNextBuffer);
  
    EntryBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer* pPrevBuffer);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
