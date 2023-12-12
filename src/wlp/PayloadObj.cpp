/*============================================================================*/
/* PayloadObj.hpp - A payload of bytes encapsulated in a proper object        */
/*============================================================================*/

#include "BASE.h"
#include "PayloadObj.hpp"
#include "WlpHeader.hpp"
#include "wlp/thrift/generated/MessageTypes_types.h"

using namespace WLP;
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(WLP,PayloadObj,CN_PAYLOADOBJ)
__aNewFunction(PayloadObj,CN_PAYLOADOBJ)
__vDeleteFunction(PayloadObj,CN_PAYLOADOBJ)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PayloadObj::PayloadObj(bool bDoNotProcess)
: Object(CN_PAYLOADOBJ)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAYLOADOBJ,_FOOTER_);

  m_nPayloadBytes = 0;
  m_uCreationTime = Hardware::uReadClock();
  m_bDoNotProcess = bDoNotProcess;
  m_caPayload[0] = 0;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caPayload, C_MAX_PAYLOAD_BYTES, CN_PAYLOADOBJ);
  _checkInitialised_(CN_PAYLOADOBJ);
#endif
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PAYLOADOBJ,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PayloadObj::~PayloadObj(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAYLOADOBJ,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a pointer to the internal payload buffer                               */
/*----------------------------------------------------------------------------*/
char* PayloadObj::pcBuf(void)
{
  return m_caPayload;
}

/*----------------------------------------------------------------------------*/
/* Get the number of bytes in the buffer                                      */
/*----------------------------------------------------------------------------*/
i8 PayloadObj::iPayloadSize(void)
{
  return m_nPayloadBytes;
}

/*----------------------------------------------------------------------------*/
/* Get the time the buffer was created                                        */
/*----------------------------------------------------------------------------*/
u8 PayloadObj::uCreationTime(void)
{
  return m_uCreationTime;
}

/*----------------------------------------------------------------------------*/
/* Get whether the wireline adaptor needs to process this message or not      */
/*----------------------------------------------------------------------------*/
bool PayloadObj::bDoNotProcess(void)
{
  return m_bDoNotProcess;
}

/*----------------------------------------------------------------------------*/
/* Set the number of bytes used in the buffer                                 */
/*----------------------------------------------------------------------------*/
void PayloadObj::vSetPayloadSize(i8 nBytes)
{
  ASSERT(nBytes >= 0);
  ASSERT(nBytes <= C_MAX_PAYLOAD_BYTES);
  m_nPayloadBytes = nBytes;
}

/*----------------------------------------------------------------------------*/
/* Set the payload buffer back to empty                                       */
/*----------------------------------------------------------------------------*/
void PayloadObj::vReset()
{
  m_nPayloadBytes = 0;
  m_caPayload[0] = 0;
}

/*----------------------------------------------------------------------------*/
/* Get a copy of the message type in this payload                             */
/*----------------------------------------------------------------------------*/
void PayloadObj::vMsgType(s256& sBuf)
{
  using namespace com::rapidsdata::wirelineprotocol::messages::v5;

  if (m_nPayloadBytes >= WLP::C_WLPHEADER_SIZE) {
    WlpHeader* pHeader = (WlpHeader*) m_caPayload;

    MessageTypes::type eMsgType = (MessageTypes::type) pHeader->iMsgType();
    std::string sMsgType = to_string(eMsgType);
    sBuf.vCopy((Stz*) sMsgType.c_str());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(PayloadObj);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
