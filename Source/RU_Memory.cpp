/*============================================================================*/
/* RU_Memory.cpp - Memory Resource Ussage                                     */
/*============================================================================*/

#include "ResourceUsage.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RU_Memory::RU_Memory(void)
: RU(CN_RUMEMORY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RUMEMORY,_FOOTER_);
  
  vInit();
  
  _checkInitialised_(CN_RUMEMORY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RUMEMORY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RU_Memory::~RU_Memory(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RUMEMORY,_FOOTER_);
  
  //vReport();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void RU_Memory::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RUMEMORY,_FOOTER_);
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    xClass[i_Class].m_nAlloc = 0;
    xClass[i_Class].m_nFree  = 0;
    xClass[i_Class].m_nMax   = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* report                                                                     */
/*----------------------------------------------------------------------------*/
void RU_Memory::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RUMEMORY,_FOOTER_);
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (! xClass[i_Class].m_nAlloc)
      continue;

    vTrace("  %lld %24s %6lld %6lld %6lld\n"_Stz,
           i_Class,
           aCN[i_Class],
           
           xClass[i_Class].m_nAlloc,
           xClass[i_Class].m_nFree,
           xClass[i_Class].m_nMax
           );
  }
}

/*----------------------------------------------------------------------------*/
/* write report                                                               */
/*----------------------------------------------------------------------------*/
void RU_Memory::vWriteReport(File* pFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RUMEMORY,_FOOTER_);
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (! xClass[i_Class].m_nAlloc)
      continue;
    
    AM::vFilePrint(pFile, "  %lld %24s %6lld %6lld %6lld\n"_Stz,
           i_Class,
           aCN[i_Class],

           xClass[i_Class].m_nAlloc,
           xClass[i_Class].m_nFree,
           xClass[i_Class].m_nMax
           );
  }
}

/*----------------------------------------------------------------------------*/
/* inc alloc                                                                  */
/*----------------------------------------------------------------------------*/
void RU_Memory::vAllocInc(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RUMEMORY,_FOOTER_);
  
  xClass[eClassNo].m_nAlloc++;
  
  i8 iDiff = xClass[eClassNo].m_nAlloc - xClass[eClassNo].m_nFree;
  if (iDiff > xClass[eClassNo].m_nMax)
    xClass[eClassNo].m_nMax = iDiff;
}

/*----------------------------------------------------------------------------*/
/* inc free                                                                   */
/*----------------------------------------------------------------------------*/
void RU_Memory::vFreeInc(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RUMEMORY,_FOOTER_);
  
  xClass[eClassNo].m_nFree++;
}

/*----------------------------------------------------------------------------*/
/* get alloc                                                                  */
/*----------------------------------------------------------------------------*/
i8 RU_Memory::nAlloc(eCN eClassNo)
{
  i8 nAlloc = xClass[eClassNo].m_nAlloc;
  
  return nAlloc;
}

/*----------------------------------------------------------------------------*/
/* get free                                                                   */
/*----------------------------------------------------------------------------*/
i8 RU_Memory::nFree(eCN eClassNo)
{
  i8 nFree = xClass[eClassNo].m_nFree;
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get max                                                                    */
/*----------------------------------------------------------------------------*/
i8 RU_Memory::nMax(eCN eClassNo)
{
  i8 nMax = xClass[eClassNo].m_nMax;
  
  return nMax;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(RU_Memory)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
