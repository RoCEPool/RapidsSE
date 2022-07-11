#include "BASE.h"

namespace BASE
{
  void  vEventLog(Stz* szFormat,...);
  void  vEventLogRead(int nMsg);
}
/*----------------------------------------------------------------------------*/
/* EventLog class contains buffer and methods for log message                 */
/*----------------------------------------------------------------------------*/
class EventLogMsg {

public:

  i8   m_iHeader;            //
  u4   m_u4Version;          // version of the event message
  i8   m_iNodeId;            // this node id
  i8   m_iSincePosixEpoch;   // timestamp of this message
  char  m_sEventMsg[C_4KB];
  i8   m_iFooter;
  char m_acSpare[16];

  EventLogMsg(void);
  ~EventLogMsg(void) noexcept(false);

};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



