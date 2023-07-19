/*============================================================================*/
/* LocalVar.cpp -- Size + Area                                                */
/*============================================================================*/

#include "LocalVar.hpp"
#include "LocalStr.hpp"
#include "LocalVars.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalVar::LocalVar(void)
{
  _nonProper_

  m_pLocalVars    = nullptr;
  m_i_LocalVar    = C_UNKNOWN;
  m_aDiscardPoint = nullptr;
  //m_xTypeValue
  m_axLocalStr    = nullptr;

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalVar::~LocalVar(void) noexcept(false)
{
  _nonProper_
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as LocalVar*                                    */
/*----------------------------------------------------------------------------*/
LocalVar::operator LocalVar*(void)
{
  _nonProper_

  return this;
}


/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
char* LocalVar::azArea(void)
{
  _nonProper_

  return m_axLocalStr->azArea();
}

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
Stz* LocalVar::szArea(void)
{
  _nonProper_

  return m_axLocalStr->szArea();
}

/*----------------------------------------------------------------------------*/
/* get discard point                                                          */
/*----------------------------------------------------------------------------*/
void* LocalVar::aDiscardPoint(void)
{
  _nonProper_

  return m_aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* get localStr                                                               */
/*----------------------------------------------------------------------------*/
LocalStr* LocalVar::axLocalStr(void)
{
  _nonProper_

  return m_axLocalStr;
}

/*----------------------------------------------------------------------------*/
/* allot a localStr                                                           */
/*----------------------------------------------------------------------------*/
LocalStr* LocalVar::axLocalStrAllot(void)
{
  _nonProper_

  //vTrace("LocalVar: %lld StrAllot\n"_Stz,m_i_LocalVar);
  
  RASSERT(! m_axLocalStr);
  m_axLocalStr = m_pLocalVars->axLocalStrAllot();
  
  m_xTypeValue.m_wxType.vSetClassNo(CN_STZVAL);
  
  return m_axLocalStr;
}

/*----------------------------------------------------------------------------*/
/* get local                                                                  */
/*----------------------------------------------------------------------------*/
i8 LocalVar::i_LocalVar(void)
{
  _nonProper_

  return m_i_LocalVar;
}


/*----------------------------------------------------------------------------*/
/* revert a localStr                                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vLocalStrRevert(void)
{
  _nonProper_

  //vTrace("LocalVar: %lld StrRevert\n"_Stz,m_i_LocalVar);
  
  m_pLocalVars->vLocalStrRevert(m_axLocalStr);
  m_axLocalStr = nullptr;
  
  m_xTypeValue.m_wxType.vSetClassNo(CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* get stack usage/required for this var                                      */
/*----------------------------------------------------------------------------*/
i8 LocalVar::jStackUsage(void)
{
  _nonProper_

  i8 jBytes;

  if (CN_STZVAL == eType()) {
    i8 jStz = jStzSize();
    i8 jSizeStz = sizeof(i8) + jStz;

    jBytes = sizeof(_Header) + jSizeStz;
  }
  else {
    jBytes = sizeof(TypeValue);
  }

  return jBytes;
}

/*----------------------------------------------------------------------------*/
/* get string size                                                            */
/*----------------------------------------------------------------------------*/
i8 LocalVar::jStzSize(void)
{
  _nonProper_

  return m_axLocalStr->jStrSize();
}

/*----------------------------------------------------------------------------*/
/* get type of this var                                                       */
/*----------------------------------------------------------------------------*/
eCN LocalVar::eType(void)
{
  _nonProper_

  return m_xTypeValue.m_wxType.eGetClassNo();
}

/*----------------------------------------------------------------------------*/
/* get pointer to type                                                        */
/*----------------------------------------------------------------------------*/
_Header* LocalVar::axType(void)
{
  _nonProper_

  return m_xTypeValue.axType();
}

/*----------------------------------------------------------------------------*/
/* get pointer to type value                                                  */
/*----------------------------------------------------------------------------*/
TypeValue* LocalVar::axTypeValue(void)
{
  _nonProper_

  return & m_xTypeValue;
}

/*----------------------------------------------------------------------------*/
/* get pointer to type                                                        */
/*----------------------------------------------------------------------------*/
Dtm* LocalVar::axValue(void)
{
  _nonProper_

  return m_xTypeValue.axValue();
}


/*----------------------------------------------------------------------------*/
/* set local variable for given typeValue                                     */
/*----------------------------------------------------------------------------*/
void LocalVar::vSet(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  eCN eType_onStack = aTypeValueArea->eType();

  if (CN_STZVAL == eType_onStack)
    vSetText(aTypeValueArea);
  else
    vSetValue(aTypeValueArea);
}


/*----------------------------------------------------------------------------*/
/* set type of local variable from given typeValue                            */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetType(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  eCN eType_onStack = aTypeValueArea->eType();
  eCN eType_ofLocal = this->eType();

  ASSERT(CN_STZVAL == eType_onStack);

  LocalStr *axLocalStr;
  if (CN_STZVAL == eType_ofLocal)
    axLocalStr = this->axLocalStr();
  else
    axLocalStr = this->axLocalStrAllot();

  if (! m_axLocalStr->m_pText)
    m_axLocalStr->m_pText = new Text;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  MemLib::vCopy(axType(),
                aTypeValueArea,
                sizeof(_Header),
                CN_STACK);

  eType_ofLocal = this->eType();
  ASSERT(CN_STZVAL == eType_ofLocal);
  //ASSERT(m_pText);
}

/*----------------------------------------------------------------------------*/
/* set text from given typeValue                                              */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetText(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  eCN eType_onStack = aTypeValueArea->eType();
  ASSERT(CN_STZVAL == eType_onStack);

  vSetType(aTypeValueArea);

  //ASSERT(m_pText);

  LocalStr* axLocalStr = this->axLocalStr();

  SizeArea* axSizeArea = axLocalStr->axSizeArea();
  byte* ayByte = reinterpret_cast<byte*>(aTypeValueArea->axValue());
  i8 nBytes = aTypeValueArea->nStackUsage() - sizeof(_Header);

  MemLib::vCopy(axSizeArea,
                ayByte,
                nBytes,
                CN_STACK);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_axLocalStr->m_pText->vSet(reinterpret_cast<Stz*>(ayByte + 8));
}

/*----------------------------------------------------------------------------*/
/* set 8B value from given typeValue                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetValue(TypeValueArea* aTypedValueArea)
{
  _nonProper_

  eCN eType_onStack = aTypedValueArea->eType();
  ASSERT(CN_STZVAL != eType_onStack);

  eCN eType_ofLocal = this->eType();

  if (CN_STZVAL == eType_ofLocal) {
    ASSERT(m_axLocalStr->m_pText);
    this->vLocalStrRevert();
   }
  else {
    //ASSERT(! m_pText);
  }

  if (m_axLocalStr && m_axLocalStr->m_pText) {
    delete m_axLocalStr->m_pText;
    m_axLocalStr->m_pText = nullptr;
  }


  ASSERT(! m_axLocalStr || ! m_axLocalStr->m_pText);

  TypeValue* axTypeValue = this->axTypeValue();

  MemLib::vCopy(axTypeValue,
                aTypedValueArea,
                sizeof(TypeValue),
                CN_STACK);

  eType_ofLocal = this->eType();
  ASSERT(CN_STZVAL != eType_ofLocal);
  ASSERT(! m_axLocalStr || ! m_axLocalStr->m_pText);
}



/*----------------------------------------------------------------------------*/
/* get local variable from given typeValue                                    */
/*----------------------------------------------------------------------------*/
void LocalVar::vGet(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  if (CN_STZVAL == this->eType()) {
    vGetType(aTypeValueArea);
    vGetText(aTypeValueArea);

  }
  else {
    vGetValue(aTypeValueArea);
  }

}

/*----------------------------------------------------------------------------*/
/* get type of local variable from given typeValue                            */
/*----------------------------------------------------------------------------*/
void LocalVar::vGetType(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  MemLib::vCopy(aTypeValueArea,
                axType(),
                sizeof(_Header),
                CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* get text                                                                   */
/*----------------------------------------------------------------------------*/
void LocalVar::vGetText(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  SizeArea* axSizeArea = axLocalStr()->axSizeArea();

  i8 jStz = this->jStzSize();
  i8 jSizeStz = sizeof(i8) + jStz;

//  MemLib::vCopy(aTypeValueArea->axValue(),
//                axSizeArea,
//                jSizeStz,
//                CN_STACK);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // todo: copy text from m_pText

  jStz = m_axLocalStr->m_pText->szGet()->nSize();
  jSizeStz = sizeof(i8) + jStz;
  MemLib::vCopy(aTypeValueArea->axValue(),
                &jStz,
                8,
                CN_STACK);
  MemLib::vCopy(reinterpret_cast<byte*>(aTypeValueArea->axValue()) + 8,
                m_axLocalStr->m_pText->szGet(),
                jStz,
                CN_STACK);

//  vTrace("Hmmmm...\n  %s\n  %s\n"_Stz,
//         aTypeValueArea->axValue(),
//         m_axLocalStr->m_pText->szGet());
//
//  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get 8B value from given typeValue                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vGetValue(TypeValueArea* aTypedValueArea)
{
  _nonProper_

  TypeValue* axTypeValue = this->axTypeValue();

  MemLib::vCopy(aTypedValueArea,
                axTypeValue,
                sizeof(TypeValue),
                CN_STACK);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
