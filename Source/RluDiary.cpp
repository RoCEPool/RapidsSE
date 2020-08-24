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
void RluDiary::vAddNote(i8 i_Sql, i8 i_Slot,
                        i8 i_EnumGroup, i8 i_Enum, i8 iItem, i8 nNanoSecs,
                        i8 iAction, i8 i_ActEnumGroup, i8 i_ActEnum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup = i_EnumGroup;
  m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum      = i_Enum;
  m_xaDiaryEnt[m_nDiaryEnt].m_iItem       = iItem;
  m_xaDiaryEnt[m_nDiaryEnt].m_nNanoSecs   = nNanoSecs;

  m_xaDiaryEnt[m_nDiaryEnt].m_iAction        = iAction;
  m_xaDiaryEnt[m_nDiaryEnt].m_i_ActEnumGroup = i_ActEnumGroup;
  m_xaDiaryEnt[m_nDiaryEnt].m_i_ActEnum      = i_ActEnum;

  if (m_xaDiaryEnt[m_nDiaryEnt].m_iAction)
    
    vTrace("[Q%lld.%lld] %lld,%lld,%lld,%s,%lld,%lld,%lld,%s\n"_Stz,
             
            i_Sql,
            i_Slot,
             
            m_nDiaryEnt,
            m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup,
            m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum,

            EnumGroups::szEnumText(m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup,
                                   m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum),

            m_xaDiaryEnt[m_nDiaryEnt].m_iItem,
            m_xaDiaryEnt[m_nDiaryEnt].m_nNanoSecs,
           
            m_xaDiaryEnt[m_nDiaryEnt].m_iAction,
            EnumGroups::szEnumText(m_xaDiaryEnt[m_nDiaryEnt].m_i_ActEnumGroup,
                                   m_xaDiaryEnt[m_nDiaryEnt].m_i_ActEnum)

            );

  else
    vTrace("[Q%lld.%lld] %lld,%lld,%lld,%s,%lld,%lld\n"_Stz,
           
          i_Sql,
          i_Slot,
           
          m_nDiaryEnt,
          m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup,
          m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum,

          EnumGroups::szEnumText(m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup,
                                 m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum),

          m_xaDiaryEnt[m_nDiaryEnt].m_iItem,
          m_xaDiaryEnt[m_nDiaryEnt].m_nNanoSecs
           
          );

  
  
  m_nDiaryEnt++;
  
  if (m_nDiaryEnt >= C_DIARY_ENTRIES)
    m_nDiaryEnt = 0;
}

/*----------------------------------------------------------------------------*/
/* write out                                                                  */
/*----------------------------------------------------------------------------*/
void RluDiary::vReport(Stz *szFilePath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);

   File* pFile = new File(nullptr);
  
  pFile->bOpenMode(szFilePath,C_OPEN_WRITE);
  
  for (i8 i_Ent = 0; i_Ent < m_nDiaryEnt; i_Ent++) {
    
    if (m_xaDiaryEnt[i_Ent].m_iAction)
      
      pFile->vPrint("%lld,%lld,%lld,%s,%lld,%lld,%lld,%s\n"_Stz,
             
                    i_Ent,
                    m_xaDiaryEnt[i_Ent].m_i_EnumGroup,
                    m_xaDiaryEnt[i_Ent].m_i_Enum,
                    
                    EnumGroups::szEnumText(m_xaDiaryEnt[i_Ent].m_i_EnumGroup,
                                          m_xaDiaryEnt[i_Ent].m_i_Enum),
                    
                    m_xaDiaryEnt[i_Ent].m_iItem,
                    m_xaDiaryEnt[i_Ent].m_nNanoSecs,
                    
                    m_xaDiaryEnt[i_Ent].m_iAction,
                    EnumGroups::szEnumText(m_xaDiaryEnt[i_Ent].m_i_ActEnumGroup,
                                          m_xaDiaryEnt[i_Ent].m_i_ActEnum)

                    );

    else
      pFile->vPrint("%lld,%lld,%lld,%s,%lld,%lld,0,n/a\n"_Stz,
             
                    i_Ent,
                    m_xaDiaryEnt[i_Ent].m_i_EnumGroup,
                    m_xaDiaryEnt[i_Ent].m_i_Enum,
                    
                    EnumGroups::szEnumText(m_xaDiaryEnt[i_Ent].m_i_EnumGroup,
                                          m_xaDiaryEnt[i_Ent].m_i_Enum),
                    
                    m_xaDiaryEnt[i_Ent].m_iItem,
                    m_xaDiaryEnt[i_Ent].m_nNanoSecs
                    );
  }

  pFile->vClose();
  delete pFile;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
