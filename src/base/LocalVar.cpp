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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get discard point                                                          */
/*----------------------------------------------------------------------------*/
void* LocalVar::aDiscardPoint(void)
{
  _nonProper_

  return m_aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* allot a localStr                                                           */
/*----------------------------------------------------------------------------*/
void LocalVar::vLocalStrAllot(void)
{
  _nonProper_

  //vTrace("LocalVar: %lld StrAllot\n"_Stz,m_i_LocalVar);
  
  RASSERT(! m_axLocalStr);
  m_axLocalStr = m_pLocalVars->axLocalStrAllot();
  
  m_xTypeValue.m_wxType.vSetClassNo(CN_STZVAL);
}

/*----------------------------------------------------------------------------*/
/* revert a localStr                                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vLocalStrRevert(void)
{
  _nonProper_

  if (CN_STZVAL != eType())
    return;

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

  i8 jStack = sizeof(TypeValue);

  if (CN_STZVAL == eType())
    jStack += jStz();

  return jStack;
}

/*----------------------------------------------------------------------------*/
/* get string size                                                            */
/*----------------------------------------------------------------------------*/
i8 LocalVar::jStz(void)
{
  _nonProper_

  return m_xTypeValue.m_wxValue.iAsI8();
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

  if (CN_STZVAL == eType_onStack) {
    vSetType(aTypeValueArea);
    vSetText(aTypeValueArea);
  }
  else {
    vLocalStrRevert();
    vSetType(aTypeValueArea);
    vSetValue(aTypeValueArea);
  }
}

/*----------------------------------------------------------------------------*/
/* set type of local variable from given typeValue                            */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetType(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  MemLib::vCopy(axType(),
                aTypeValueArea,
                sizeof(_Header),
                CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* set text from given typeValue                                              */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetText(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  ASSERT(CN_STZVAL == aTypeValueArea->eType());

  vEnsureText();

  i8 jBytes_typeValueStz = aTypeValueArea->nStackUsage();
  i8 jBytes_valueStz = jBytes_typeValueStz - sizeof(_Header);
  i8 jBytes_stz = jBytes_valueStz - sizeof(i8);

  vSetSize(jBytes_stz);

  m_axLocalStr->m_pText->vSet(reinterpret_cast<Stz*>(aTypeValueArea->szStz()));
}

/*----------------------------------------------------------------------------*/
/* set 8B value from given typeValue                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetValue(TypeValueArea* aTypedValueArea)
{
  _nonProper_

  ASSERT(! m_axLocalStr || ! m_axLocalStr->m_pText);

  TypeValue* axTypeValue = this->axTypeValue();

  MemLib::vCopy(axTypeValue->axValue(),
                aTypedValueArea->axValue(),
                sizeof(Dtm),
                CN_STACK);

  ASSERT(CN_STZVAL != this->eType());
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
    vGetType(aTypeValueArea);
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


  i8 jStz = this->jStz();
  RASSERT(jStz == m_xTypeValue.m_wxValue.iAsI8());

  i8 jSizeStz = sizeof(i8) + jStz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // todo: copy text from m_pText

  jStz = m_axLocalStr->m_pText->szGet()->nSize();
  jSizeStz = sizeof(i8) + jStz;
  MemLib::vCopy(aTypeValueArea->axValue(),
                &jStz,
                8,
                CN_STACK);

  i8 jStz_this = m_xTypeValue.axValue()->iAsI8();
  RASSERT(jStz_this == jStz);

  MemLib::vCopy(//reinterpret_cast<byte*>(aTypeValueArea->axValue()) + 8,
                aTypeValueArea->szStz(),
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
/* get 8B value from this localVar into given typedValueArea                  */
/*----------------------------------------------------------------------------*/
void LocalVar::vGetValue(TypeValueArea* aTypedValueArea)
{
  _nonProper_

  MemLib::vCopy(aTypedValueArea->axValue(),
                axValue(),
                sizeof(Dtm),
                CN_STACK);
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ensure text                                                                */
/*----------------------------------------------------------------------------*/
void LocalVar::vEnsureText(void)
{
  _nonProper_

  if (! m_axLocalStr)
    vLocalStrAllot();

  m_axLocalStr->vEnsureText();
}

/*----------------------------------------------------------------------------*/
/* set value given size argument                                              */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetSize(i8 jStz)
{
  _nonProper_

  vSetValue(jStz);
}

/*----------------------------------------------------------------------------*/
/* set value given argument                                                   */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetValue(i8 iValue)
{
  _nonProper_

  m_xTypeValue.axValue()->vAsI8(iValue);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
