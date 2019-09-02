/*============================================================================*/
/* eOC.h -- Other Characters                                                  */
/*============================================================================*/
#pragma once

enum eOC // other characters
{
  /*  0 */ OC_NONE=0,
  
  /*    */ OC_ASSIGN,
  /*    */ OC_ASSIGNED,
  /*    */ OC_UNASSIGN,
  /*    */ OC_COMMAND,
  /*    */ OC_CONSTRUCT,
  /*    */ OC_DESTRUCT,
  /*    */ OC_BLOCKED,
  /*    */ OC_UNBLOCKED,
  /*    */ OC_WAIT,
  /*    */ OC_RESUME,
  /*    */ OC_SPAWN,
  /*    */ OC_SCHEDULE,
  /*    */ OC_SIGNAL,
  /*    */ OC_SIGNALED,
  /*    */ OC_GRAB,
  /*    */ OC_DROP,
  /*    */ OC_TRANSITION,
  /*    */ OC_REPEAT,
  /*    */ OC_SLEEP,
  /*    */ OC_SEND,
  /*    */ OC_RECEIVE,
  /*    */ OC_EXIT,
  /*    */ OC_INDEFINITE,
  /*    */ OC_NOTE,
  /*    */ OC_SPACE,
  /*    */ OC_APPEND,
  /*    */ OC_IDLE,
  /*    */ OC_AWAKE,
  /*    */ OC_INFINITE,
  /*    */ OC_LOWPRIORITY,
  /*    */ OC_SERVICE,
  /*    */ OC_ADD,
  /*    */ OC_OPEN,
  /*    */ OC_CLOSE,
  /*    */ OC_READ,
  /*    */ OC_WRITE,
  /*    */ OC_REQUIRES,

  /*    */ OC_JOB,
  /*    */ OC_LOCK,
  /*    */ OC_REQUEST,
  /*    */ OC_MUTEX,
  /*    */ OC_SESSION,
  /*    */ OC_SOCKET,
  /*    */ OC_PROMPT,
  /*    */ OC_TASK,
  /*    */ OC_THREAD,

  /*    */ OC_ENUMS,
};

static Stz* aOC[]
{
   /*  0 */ "❔"_Stz,
  
   /*    */ "💲"_Stz, // assign
   /*    */ "✅"_Stz, // assigned
   /*    */ "⏏️"_Stz, // unassign
   /*    */ "🛎"_Stz, // command
   /*    */ "🌻"_Stz, // construct
   /*    */ "🗑"_Stz, // destruct
   /*    */ "🚧"_Stz, // blocked
   /*    */ "🎢"_Stz, // unblocked
   /*    */ "🧘‍♀️"_Stz, // wait
   /*    */ "🏃‍♀️"_Stz, // resume
   /*    */ "📌"_Stz, // spawn
   /*    */ "🍏"_Stz, // schedule
   /*    */ "📣"_Stz, // signal
   /*    */ "🙉"_Stz, // signaled
   /*    */ "❌"_Stz, // grab
   /*    */ "⭕️"_Stz, // drop
   /*    */ "🌗"_Stz, // transition
   /*    */ "🔃"_Stz, // repeat
   /*    */ "💤"_Stz, // sleep
   /*    */ "📦"_Stz, // send
   /*    */ "📥"_Stz, // recv
   /*    */ "🚪"_Stz, // exit
   /*    */ "🛑"_Stz, // indefinite
   /*    */ "🎵"_Stz, // note
   /*    */ "🍭"_Stz, // space
   /*    */ "👯‍♂️"_Stz, // append
   /*    */ "🎣"_Stz, // idle
   /*    */ "😳"_Stz, // awake
   /*    */ "♾"_Stz, // infinite
   /*    */ "🐢"_Stz, // lowprority
   /*    */ "👻"_Stz, // service
   /*    */ "➕"_Stz, // add
   /*    */ "📂"_Stz, // open
   /*    */ "🗄"_Stz, // close
   /*    */ "🦉"_Stz, // read
   /*    */ "📝"_Stz, // write
   /*    */ "🧮"_Stz, // requires

   /*    */ "💈"_Stz, // job
   /*    */ "🔒"_Stz, // lock
   /*    */ "🛎"_Stz, // request
   /*    */ "🚥"_Stz, // mutex
   /*    */ "🎤"_Stz, // session
   /*    */ "🔌"_Stz, // socket
   /*    */ "❓"_Stz, // prompt
   /*    */ "📎"_Stz, // task
   /*    */ "〰️"_Stz, // thread


   /*    */ "ENUMS"_Stz,
};

//static Stz* aOC[]
//{
//  /*  0 */ "NONE"_Stz,
//  /*    */ "⬛️"_Stz,
//  /*    */ "⬜️"_Stz,
//  /*    */ "🛑"_Stz,
//  /*    */ "🅰️"_Stz,
//  /*    */ "🅱️"_Stz,
//  /*    */ "🔵"_Stz,
//  /*    */ "⚫️"_Stz,
//  /*    */ "⚪️"_Stz,
//  /*    */ "🔴"_Stz,
//  /*    */ "⭕️"_Stz,
//  /*    */ "❌"_Stz,
//  /*    */ "💢"_Stz,
//  /*    */ "↙️"_Stz,
//  /*    */ "↘️"_Stz,
//  /*    */ "🔷"_Stz,
//  /*    */ "🔶"_Stz,
//  /*    */ "♠️"_Stz,
//  /*    */ "♣️"_Stz,
//  /*    */ "♥️"_Stz,
//  /*    */ "♦️"_Stz,
//  /*    */ "🆗"_Stz,
//  /*    */ "0️⃣"_Stz,
//  /*    */ "9️⃣"_Stz,
//  /*    */ "#️⃣"_Stz,
//  /*    */ "*️⃣"_Stz,
//  /*    */ "📊"_Stz,
//  /*    */ "📈"_Stz,
//  /*    */ "📉"_Stz,
//  /*    */ "✅"_Stz,
//  /*    */ "✅"_Stz,
//  /*    */ "❎"_Stz,
//  /*    */ "✳️"_Stz,
//  /*    */ "🚹"_Stz,
//  /*    */ "🚺"_Stz,
//  /*    */ "⚛️"_Stz,
//  /*    */ "❗️"_Stz,
//  /*    */ "‼️"_Stz,
//  /*    */ "❓"_Stz,
//  /*    */ "⁉️"_Stz,
//  /*    */ "➕"_Stz,
//  /*    */ "➖"_Stz,
//  /*    */ "➗"_Stz,
//  /*    */ "✖️"_Stz,
//  /*    */ "♾"_Stz,
//  /*    */ "💲"_Stz,
//  /*    */ "🔒"_Stz,
//  /*    */ "🆔"_Stz,
//  /*    */ "☮️"_Stz,
//  /*    */ "⏹"_Stz,
//  /*    */ "▶️"_Stz,
//  /*    */ "☂️"_Stz,
//  /*    */ "🌂"_Stz,
//  /*    */ "🗝"_Stz,
//  /*    */ "🧘‍♀️"_Stz,
//  /*    */ "🏃‍♀️"_Stz,
//  /*    */ "🤹‍♂️"_Stz,
//  /*    */ "🤹‍♂️"_Stz,
//  /*    */ "⚡️"_Stz,
//  /*    */ "🌪"_Stz,
//  /*    */ "🌈"_Stz,
//  /*    */ "🌤"_Stz,
//  /*    */ "♠️"_Stz,
//  /*    */ "❤️"_Stz,
//  /*    */ "🍏"_Stz,
//  /*    */ "📌"_Stz,
//  /*    */ "🚀"_Stz,
//  /*    */ "📣"_Stz,
//  /*    */ "🙉"_Stz,
//  /*    */ "🎈"_Stz,
//  /*    */ "🍾"_Stz,
//  /*    */ "👁‍🗨"_Stz,
//  /*    */ "〰️"_Stz,
//  /*    */ "➰"_Stz,
//  /*    */ "▫️"_Stz,
//  /*    */ "✏️"_Stz,
//
//  /*    */ "ENUMS"_Stz,
//};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
