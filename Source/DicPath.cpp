/*============================================================================*/
/* DicPath.hpp -- Dictionary Path                                             */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DicPath* SenVM::_pDicPath(void* aAddr)
{
  DicPath* pDicPath = reinterpret_cast<DicPath*>(aAddr);
  
  RASSERT_TYPE(pDicPath,CN_DICPATH);
  
  return pDicPath;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DicPath::operator new(size_t)
{
  DicPath* pDicPath = (DicPath*)(HeapMem::pAllocate(CN_DICPATH));
  
  return pDicPath;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DicPath::operator delete(void* pDicAddress) 
{
  DicPath* pDicPath = reinterpret_cast<DicPath*>(pDicAddress);
  
  HeapMem::vFree(pDicPath);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DicPath::DicPath(void)
: Object(CN_DICPATH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  m_nDicNames = -1;
  
  for (i8 i_Dic = 0; i_Dic < C_DIC_PATHDEPTH; i_Dic++)
    m_sDicName[i_Dic] = ""_Stz;
  
  _checkInitialised_(CN_DICPATH);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DicPath::~DicPath(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump stack of dictionary paths                                             */
/*----------------------------------------------------------------------------*/
void DicPath::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  for (i8 i_Dic = m_nDicNames; i_Dic >= 0; i_Dic--)
    vTrace("Dic %lld %s\n"_Stz,i_Dic,m_sDicName[i_Dic].azChar());
}

/*----------------------------------------------------------------------------*/
/* get number of dictionary names in path                                     */
/*----------------------------------------------------------------------------*/
i8 DicPath::nDicNames(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  return m_nDicNames;
}

/*----------------------------------------------------------------------------*/
/* get name of given dictionary                                               */
/*----------------------------------------------------------------------------*/
s32& DicPath::sDicName(i8 i_Dic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  return m_sDicName[i_Dic];
}

/*----------------------------------------------------------------------------*/
/* set name of given dictionary                                               */
/*----------------------------------------------------------------------------*/
void DicPath::vDicName(i8 i_Dic, s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  m_sDicName[i_Dic] = sName;
  
  if (i_Dic + 1 > m_nDicNames) {
    m_nDicNames = i_Dic + 1;
    ASSERT (m_nDicNames <= C_DIC_PATHDEPTH);
  }
}

/*----------------------------------------------------------------------------*/
/* get entire path as a single string                                         */
/*----------------------------------------------------------------------------*/
void DicPath::vAsString(s256& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  sString = ""_Stz;

  for (i8 i_Name = 0; i_Name < m_nDicNames; i_Name++) {
    if (i_Name)
      sString.vAppend('.');
    sString.vConcat(m_sDicName[i_Name]);
  }
}

/*----------------------------------------------------------------------------*/
/* append name to path                                                        */
/*----------------------------------------------------------------------------*/
void DicPath::vAppendName(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
 
  m_sDicName[m_nDicNames++] = sName;
  
  ASSERT (m_nDicNames <= C_DIC_PATHDEPTH);
}

/*----------------------------------------------------------------------------*/
/* set path from single string                                                */
/*----------------------------------------------------------------------------*/
void DicPath::vSetPath(s256& sPathString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  m_nDicNames = 0;
  
  s32 sFolderNm;
  s256 sRemainingPath;
  
  sRemainingPath = sPathString;
  do {
    
    StrLib::vCopySplit(sFolderNm.azChar(),
                       sRemainingPath.azChar(),
                       sRemainingPath.azChar(),
                       '.');
    
    vAppendName(sFolderNm);
    
  } while (*sRemainingPath.azChar());
  
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(DicPath)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
