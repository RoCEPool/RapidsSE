
#include "EventLog.hpp"


/*----------------------------------------------------------------------------*/
/* report an event message                                                    */
/*----------------------------------------------------------------------------*/
void BASE::vSigEv(Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);
  AM::vWriteEvent_vargs(aFormat,xVarArgs);
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* read event messages                                                        */
/*----------------------------------------------------------------------------*/
void BASE::vSigEvRead(int nMsg)
{
  AM::vReadEventMsg(nMsg);
}


EventLogMsg::EventLogMsg(void)
{
  m_u4Version = 0;
  m_iNodeId = 0;
  m_iSincePosixEpoch = 0;
}
EventLogMsg::~EventLogMsg(void) noexcept(false)
{}
