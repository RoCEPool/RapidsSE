/*============================================================================*/
/* RluDiary.cpp -- Rlu Diary                                                  */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,RluDiary,CN_RLUDIARY)
__aNewFunction(RluDiary,CN_RLUDIARY)
__vDeleteFunction(RluDiary,CN_RLUDIARY)
__aFooterFunction(RluDiary)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RluDiary::RluDiary(void)
: Object(CN_RLUDIARY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  m_nDiaryEnt = 0;
  
  m_sDiaryNm = "DiaryNm"_Stz;
  m_sTitle   = "Diary Report"_Stz;
  
  m_sJobNote = "--"_Stz;
  
  MemLib::vZero(&m_xaDiaryEnt,sizeof(m_xaDiaryEnt),CN_RLUDIARY);

  _checkInitialised_(CN_RLUDIARY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RluDiary::~RluDiary(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write out                                                                  */
/*----------------------------------------------------------------------------*/
void RluDiary::vAddNote(i8 i_Sql, i8 i_Slot, i8 nQueueLength,
                        i8 i_EnumGroup, i8 i_Enum, i8 iItem, i8 nNanoSecs,
                        i8 iAction, i8 i_ActEnumGroup, i8 i_ActEnum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  m_xaDiaryEnt[m_nDiaryEnt].m_iThreadNo      = AM::iThisTLSThreadNo();
  m_xaDiaryEnt[m_nDiaryEnt].m_nQueueLength   = nQueueLength;

  m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup    = i_EnumGroup;
  m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum         = i_Enum;
  m_xaDiaryEnt[m_nDiaryEnt].m_iItem          = iItem;
  m_xaDiaryEnt[m_nDiaryEnt].m_nNanoSecs      = nNanoSecs;

  m_xaDiaryEnt[m_nDiaryEnt].m_iAction        = iAction;
  m_xaDiaryEnt[m_nDiaryEnt].m_i_ActEnumGroup = i_ActEnumGroup;
  m_xaDiaryEnt[m_nDiaryEnt].m_i_ActEnum      = i_ActEnum;

//  if (401 == i_Sql) {
//    if (m_xaDiaryEnt[m_nDiaryEnt].m_iAction)
//
//      vTrace("[Q%lld.%lld] %lld,%lld,%lld,%s,%lld,%lld,%lls,%s\n"_Stz,
//
//              i_Sql,
//              i_Slot,
//
//              m_nDiaryEnt,
//              m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup,
//              m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum,
//
//              EnumGroups::szEnumText(m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup,
//                                     m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum),
//
//              m_xaDiaryEnt[m_nDiaryEnt].m_iItem,
//              m_xaDiaryEnt[m_nDiaryEnt].m_nNanoSecs,
//
//              aPP[m_xaDiaryEnt[m_nDiaryEnt].m_iAction],
//              EnumGroups::szEnumText(m_xaDiaryEnt[m_nDiaryEnt].m_i_ActEnumGroup,
//                                     m_xaDiaryEnt[m_nDiaryEnt].m_i_ActEnum)
//
//              );
//
//    else {
//      vTrace("[Q%lld.%lld] %lld,%lld,%lld,%s,%lld,%lld\n"_Stz,
//
//            i_Sql,
//            i_Slot,
//
//            m_nDiaryEnt,
//            m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup,
//            m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum,
//
//            EnumGroups::szEnumText(m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup,
//                                   m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum),
//
//            m_xaDiaryEnt[m_nDiaryEnt].m_iItem,
//            m_xaDiaryEnt[m_nDiaryEnt].m_nNanoSecs
//
//            );
//    }
//  }
  
  m_nDiaryEnt++;
  
  if (m_nDiaryEnt >= C_DIARY_ENTRIES)
    m_nDiaryEnt = 0;
}

/*----------------------------------------------------------------------------*/
/* diary name                                                                 */
/*----------------------------------------------------------------------------*/
Stz* RluDiary::szDiaryNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  return m_sDiaryNm;
}

/*----------------------------------------------------------------------------*/
/* diary name                                                                 */
/*----------------------------------------------------------------------------*/
void RluDiary::vDiaryNm(Stz* szDiaryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  m_sDiaryNm = szDiaryNm;
}

/*----------------------------------------------------------------------------*/
/* job note                                                                   */
/*----------------------------------------------------------------------------*/
Stz* RluDiary::szJobNote(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  return m_sJobNote;
}

/*----------------------------------------------------------------------------*/
/* job note                                                                   */
/*----------------------------------------------------------------------------*/
void RluDiary::vJobNote(Stz* szJobNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  m_sJobNote = szJobNote;
}

/*----------------------------------------------------------------------------*/
/* write out                                                                  */
/*----------------------------------------------------------------------------*/
void RluDiary::vReportDiary(Stz *szFilePath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);

  File* pFile = new File(nullptr);
  
  pFile->bOpenMode(szFilePath,C_OPEN_WRITE);
  
  s256 sSummary;
  
  pFile->vPrint("%s [%s %s]\n"_Stz,
                m_sTitle.azChar(),
                AM::szSummary(sSummary),
                m_sJobNote.azChar()
                );

  for (i8 i_Ent = 0; i_Ent < m_nDiaryEnt; i_Ent++) {
    
    if (m_xaDiaryEnt[i_Ent].m_iAction)
      
      pFile->vPrint("%lld,%lld,%lld,%lld,%lld,%s,%lld,%lld,%lls,%s\n"_Stz,
             
                    i_Ent,
                    (i8)m_xaDiaryEnt[i_Ent].m_iThreadNo,
                    (i8)m_xaDiaryEnt[i_Ent].m_nQueueLength,

                    (i8)m_xaDiaryEnt[i_Ent].m_i_EnumGroup,
                    (i8)m_xaDiaryEnt[i_Ent].m_i_Enum,
                    EnumGroups::szEnumText(m_xaDiaryEnt[i_Ent].m_i_EnumGroup,
                                          m_xaDiaryEnt[i_Ent].m_i_Enum),
                    
                    (i8)m_xaDiaryEnt[i_Ent].m_iItem,
                    (i8)m_xaDiaryEnt[i_Ent].m_nNanoSecs,
                    
                    aPP[m_xaDiaryEnt[i_Ent].m_iAction],
                    EnumGroups::szEnumText(m_xaDiaryEnt[i_Ent].m_i_ActEnumGroup,
                                          m_xaDiaryEnt[i_Ent].m_i_ActEnum)

                    );

    else
      pFile->vPrint("%lld,%lld,%lld,%lld,%lld,%s,%lld,%lld,0,n/a\n"_Stz,
             
                    i_Ent,
                    (i8)m_xaDiaryEnt[i_Ent].m_iThreadNo,
                    (i8)m_xaDiaryEnt[i_Ent].m_nQueueLength,

                    (i8)m_xaDiaryEnt[i_Ent].m_i_EnumGroup,
                    (i8)m_xaDiaryEnt[i_Ent].m_i_Enum,
                    EnumGroups::szEnumText(m_xaDiaryEnt[i_Ent].m_i_EnumGroup,
                                          m_xaDiaryEnt[i_Ent].m_i_Enum),
                    
                    (i8)m_xaDiaryEnt[i_Ent].m_iItem,
                    (i8)m_xaDiaryEnt[i_Ent].m_nNanoSecs
                    );
  }

  pFile->vClose();
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* title                                                                      */
/*----------------------------------------------------------------------------*/
Stz* RluDiary::szTitle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  return m_sTitle;
}

/*----------------------------------------------------------------------------*/
/* title                                                                      */
/*----------------------------------------------------------------------------*/
void RluDiary::vTitle(Stz* szTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  m_sTitle = szTitle;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
