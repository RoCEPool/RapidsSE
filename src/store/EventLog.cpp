

#include "EventLog.hpp"



/*----------------------------------------------------------------------------*/
/* report an event message                                                    */
/*----------------------------------------------------------------------------*/
void BASE::vEventLog(Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);
  AM::vWriteEvent_vargs(aFormat,xVarArgs);
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* read event messages                                                        */
/*----------------------------------------------------------------------------*/
void BASE::vEventLogRead(int nMsg)
{
  AM::vReadEventMsg(nMsg);
}


EventLogMsg::EventLogMsg(void)
{
  m_iHeader = 0;
  m_u4Version = 0;
  m_iNodeId = 0;
  m_iSincePosixEpoch = 0;
  m_sEventMsg[0] = '\0';
  m_iFooter = 0;
  m_acSpare[0] = '\0';
}
EventLogMsg::~EventLogMsg(void) noexcept(false)
{}
