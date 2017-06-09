/*============================================================================*/
/* EntryBuffer.hpp -- Entry Buffer                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
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
  
  class EntryBuffer : public Buffer
  {
  protected:
    eBD   m_eDurability; // buffer durability
    i8    m_nEntries;    // number of entries in this buffer
  
    EntryBuffer*   m_pPrevBuffer;  // prev buffer
    EntryBuffer*   m_pNextBuffer;  // next buffer
    EntryBuffer*   m_pHigherLevel; // higher level buffer that points to this buffer
  
  public:
    EntryBuffer(eCN eClassNo);
  
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
