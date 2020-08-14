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
void RluDiary::vAddNote(i8 i_EnumGroup, i8 i_Enum, i8 iItem, i8 nNanoSecs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RLUDIARY,_FOOTER_);
  
  m_xaDiaryEnt[m_nDiaryEnt].m_i_EnumGroup = i_EnumGroup;
  m_xaDiaryEnt[m_nDiaryEnt].m_i_Enum      = i_Enum;
  m_xaDiaryEnt[m_nDiaryEnt].m_iItem       = iItem;
  m_xaDiaryEnt[m_nDiaryEnt].m_nNanoSecs   = nNanoSecs;

  m_nDiaryEnt++;
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
    
    pFile->vPrint(" %lld,%lld,%lld,%s,%lld,%lld\n"_Stz,
           
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
