#include "BASE.h"

namespace BASE
{
  void  vSigEv(Stz* szFormat,...);
  void  vSigEvRead(int nMsg);
}
/*----------------------------------------------------------------------------*/
/* EventLog class contains buffer and methods for log message                 */
/*----------------------------------------------------------------------------*/
class EventLogMsg {

public:

  u4   m_u4Version;          // version of the event message
  i8   m_iNodeId;            // this node id
  i8   m_iSincePosixEpoch;   // timestamp of this message
  s256  m_sEventMsg;

  EventLogMsg(void);
  ~EventLogMsg(void) noexcept(false);

};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



