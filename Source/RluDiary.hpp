/*============================================================================*/
/* RluDiary.hpp -- Rlu Diary                                                  */
/*============================================================================*/
#pragma once

namespace Expressions
{
  struct RluDiaryEnt
  {
    i2 m_i_EnumGroup;
    i2 m_i_Enum;
    i2 m_iItem;
    i8 m_nNanoSecs;
  
    i2 m_iAction;
    i2 m_i_ActEnumGroup;
    i2 m_i_ActEnum;

  };

  static const i8 C_DIARY_ENTRIES =
    (C_SIZE_LARGE_OBJECT - C_16KB) / sizeof(RluDiaryEnt);

  class RluDiary : public Object
  {
  public:
  
    i8 m_nDiaryEnt;
  
    RluDiaryEnt m_xaDiaryEnt[C_DIARY_ENTRIES];
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RluDiary(void);
   ~RluDiary(void) noexcept(false);
  
    void vAddNote(i8 i_Sql, i8 i_Slot,
                  i8 i_EnumGroup, i8 i_Enum, i8 iItem, i8 i_nNanoSecs,
                  
                  i8 iAction, i8 i_ActEnumGroup, i8 i_ActEnum
                  );

    void vReport(Stz* szFilePath);
    
  private:
    Footer* aFooter(void);
  };
  
  RluDiary* _pRluDiary(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
