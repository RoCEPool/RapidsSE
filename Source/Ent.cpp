/*============================================================================*/
/* Ent.cpp -- Index Entry                                                     */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Ent::operator new(size_t)
{
  vFatal("fatal: Ent class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Ent::operator new(size_t, Ent* aEnt)
{
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Ent::Ent(eCN eClassNo, i8 nSizeEnt_Without_NEO)
: Header(eClassNo)
{
  m_nSizeEnt_Without_NEO = static_cast<i4>(nSizeEnt_Without_NEO);
  
  m_nKeyBytes = static_cast<i4>(nSizeEnt_Without_NEO - sizeof(Ent));
  
#ifdef WITH_DEBUG_VERSION
  m_iSkipNo = -1;
#endif
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Ent::~Ent(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get id                                                                     */
/*----------------------------------------------------------------------------*/
Id& Ent::xId(void)
{
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set id                                                                     */
/*----------------------------------------------------------------------------*/
void Ent::vId(Id xId)
{
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
byte* Ent::ayKeyBytes(void)
{
  byte* ayKey = aAdd<byte*>(this,sizeof(Ent));
  
  return pPoolCheck<byte*>(ayKey);
}

/*----------------------------------------------------------------------------*/
/* get number of levels                                                       */
/*----------------------------------------------------------------------------*/
i8 Ent::nLevels(void)
{
  return m_xId.nLevels();
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void Ent::vLevels(i8 nLevels)
{
  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get a next entry offset                                                    */
/*----------------------------------------------------------------------------*/
i4 Ent::iNEO(i8 i_Level)
{
  ASSERT(i_Level >= 0 && i_Level < nLevels());
  
  //i8 i_NEO = m_nSizeEnt_Without_NEO + (C_NEO_SIZE * i_Level);
  //RASSERT(i_NEO == sizeof(Ent) + m_nKeyBytes + (C_NEO_SIZE * i_Level));
  
  i8 i_NEO = sizeof(Ent) + m_nKeyBytes + (C_NEO_SIZE * i_Level);
  
  i4 iNEO = * (aAdd<i4*>(this,i_NEO));
  
  return iNEO;
}

/*----------------------------------------------------------------------------*/
/* set a next entry offset                                                    */
/*----------------------------------------------------------------------------*/
void Ent::vNEO(i8 i_Level, i4 i_Offset)
{
  ASSERT(i_Level >= 0 && i_Level < nLevels());
  
  //i8 i_NEO = m_nSizeEnt_Without_NEO + (C_NEO_SIZE * i_Level);
  //RASSERT(i_NEO == sizeof(Ent) + m_nKeyBytes + (C_NEO_SIZE * i_Level));

  i8 i_NEO = sizeof(Ent) + m_nKeyBytes + (C_NEO_SIZE * i_Level);

  * (aAdd<i4*>(this,i_NEO)) = i_Offset;
}

/*----------------------------------------------------------------------------*/
/* dump the NEO after this entry                                              */
/*----------------------------------------------------------------------------*/
void Ent::vNEOdump(void)
{
  NEO* aNEO = this->aNEO();
  vTrace("Ent %p levels: %lld NEO: %p"_Stz,
         this,nLevels(),aNEO);
  
  for (i8 i_Level = 0; i_Level < nLevels(); i_Level++)
    vTrace(" %lld: %d"_Stz,i_Level,aNEO->m_iOffset[i_Level]);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get size of entry with neo                                                 */
/*----------------------------------------------------------------------------*/
i8 Ent::nSizeBytes(void)
{
  //i8 nBytes = m_nSizeEnt_Without_NEO + nSizeNeo();
  //RASSERT(nBytes == sizeof(Ent) + m_nKeyBytes + nSizeNeo());

  i8 nBytes = sizeof(Ent) + m_nKeyBytes + nSizeNeo();

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get size key                                                               */
/*----------------------------------------------------------------------------*/
i8 Ent::nSizeKey(void)
{
  RASSERT(m_nKeyBytes == m_nSizeEnt_Without_NEO - sizeof(Ent));
  
  return (i8)(m_nKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* get size of the NEO after this entry                                       */
/*----------------------------------------------------------------------------*/
i8 Ent::nSizeNeo(void)
{
  i8 nLvls  = nLevels();
  i8 nBytes = C_NEO_SIZE * nLvls;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* test for valid entry                                                       */
/*----------------------------------------------------------------------------*/
bool Ent::bValidEnt(void)
{  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if ( ! (CN_ENT == eClass() || CN_FREEITEM == eClass()) ) {
    vTrace("Not valid class\n"_Stz);
    return false;
  }
  
#ifdef USE_LW_FOOOTER
  if (! aFooter()->bValidFooter()) {
    vTrace("Not valid footer\n"_Stz);
    return false;
  }
#endif
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next entry offset array                                     */
/*----------------------------------------------------------------------------*/
NEO* Ent::aNEO(void)
{
  i8 i_NEO = sizeof(Ent) + m_nKeyBytes;

  NEO* aNEO = aAdd<NEO*>(this,i_NEO);
  
  return aNEO;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
