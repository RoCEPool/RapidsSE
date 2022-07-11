#include "BASE.h"

namespace BASE
{
  void  vEventLog(Stz* szFormat,...);
}
/*----------------------------------------------------------------------------*/
/* EventLog class contains buffer and methods for log message                 */
/*----------------------------------------------------------------------------*/
class EventLogMsg {

public:

  i8   m_iHeader;            //

  u4   m_u4Version;          // version of the event message
  u2   m_u2DataLen;          // length of data  includes size of itself
  i8   m_iNodeId;            // this node id
  i8   m_iSincePosixEpoch;   // timestamp of this message
  char  m_sEventMsg[C_4KB];
  i8   m_iFooter;            //   8
  char m_acSpare[16];        // 16

};




/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



