/*============================================================================*/
/* RluDiary.hpp -- Rlu Diary                                                  */
/*============================================================================*/
#pragma once

namespace Expressions
{
  struct RluDiaryEnt
  {
    i8 m_iThreadNo;
    i8 m_nQueueLength;

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
  
    s32  m_sDiaryNm;
    s256 m_sDiaryDir;
    s256 m_sTitle;
    s256 m_sJobNote;

    RluDiaryEnt m_xaDiaryEnt[C_DIARY_ENTRIES];
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RluDiary(void);
   ~RluDiary(void) noexcept(false);
  
    void vAddNote(i8 i_Sql, i8 i_TablePtn, i8 nQueueLength,
                  i8 i_EnumGroup, i8 i_Enum, i8 iItem, i8 i_nNanoSecs,
                  
                  i8 iAction, i8 i_ActEnumGroup, i8 i_ActEnum
                  );

    Stz* szDiaryDir(void);
    void vDiaryDir(Stz* szDiaryDir);
    
    Stz* szDiaryNm(void);
    void vDiaryNm(Stz* szDiaryNm);
  
    void vReportDiary(Stz* szFilePath);
  
    Stz* szJobNote(void);
    void vJobNote(Stz* szJobNote);

    Stz* szTitle(void);
    void vTitle(Stz* szTitle);
    
  private:
    Footer* aFooter(void);
  };
  
  RluDiary* _pRluDiary(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
