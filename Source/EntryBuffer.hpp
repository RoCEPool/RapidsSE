/*============================================================================*/
/* RowBuffer.hpp -- Entry Buffer                                              */
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
  private:
    eBD   m_eDurability; // buffer durability
  
  public:
    EntryBuffer(eCN eClassNo);
  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
