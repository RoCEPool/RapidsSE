/*============================================================================*/
/* eOC.h -- Other Characters                                                  */
/*============================================================================*/
#pragma once

enum eOC // other characters
{
  /*  0 */ OC_NONE=0,
  
  /*    */ OC_ACCEPT,
  /*    */ OC_ACK,
  /*    */ OC_PAIR,
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
  /*    */ OC_REMOVE,
  /*    */ OC_OPEN,
  /*    */ OC_CLOSE,
  /*    */ OC_READ,
  /*    */ OC_WRITE,
  /*    */ OC_REQUIRES,
  /*    */ OC_SECURE,
  /*    */ OC_UNSECURE,
  /*    */ OC_RUN,
  /*    */ OC_MOVE,
  /*    */ OC_GET,
  /*    */ OC_POP,
  /*    */ OC_FIND,
  /*    */ OC_FOUND,
  /*    */ OC_NOTFOUND,
  /*    */ OC_CLARITY,
  /*    */ OC_SOLIDITY,
  /*    */ OC_CONFUSION,
  /*    */ OC_TRIGGER,
  /*    */ OC_BIDE,
  /*    */ OC_END,
  /*    */ OC_BEGIN,
  /*    */ OC_ENTER,
  /*    */ OC_CONNECT,
  /*    */ OC_TRACE,
  /*    */ OC_PHASE,
  /*    */ OC_SEED,
  /*    */ OC_CHECK,

  /*    */ OC_JOB,
  /*    */ OC_LOCK,
  /*    */ OC_REQUEST,
  /*    */ OC_MUTEX,
  /*    */ OC_SESSION,
  /*    */ OC_SOCKET,
  /*    */ OC_PROMPT,
  /*    */ OC_TASK,
  /*    */ OC_THREAD,

  /*    */ OC_CURSOR,

  /*    */ OC_ENUMS,
};

static Stz* aOC[]
{
   /*  0 */ "❔"_Stz,
  
   /*    */ "🥳"_Stz, // accepted
   /*    */ "✅"_Stz, // ack
   /*    */ "🍐"_Stz, // pair
   /*    */ "⏏️"_Stz, // unassign
   /*    */ "🛎"_Stz, // command
   /*    */ "🌻"_Stz, // construct
   /*    */ "🗑"_Stz, // destruct
   /*    */ "🚧"_Stz, // blocked
   /*    */ "🎢"_Stz, // unblocked
   /*    */ "🧘‍♀️"_Stz, // wait
   /*    */ "🛶"_Stz, // resume
   /*    */ "🐠"_Stz, // spawn
   /*    */ "🔮"_Stz, // schedule
   /*    */ "📣"_Stz, // signal
   /*    */ "🙉"_Stz, // signaled
   /*    */ "❌"_Stz, // grab
   /*    */ "⭕️"_Stz, // drop
   /*    */ "🐛"_Stz, // transition
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
   /*    */ "➖"_Stz, // remove
   /*    */ "📂"_Stz, // open
   /*    */ "🗄"_Stz, // close
   /*    */ "🦉"_Stz, // read
   /*    */ "📝"_Stz, // write
   /*    */ "🧮"_Stz, // requires
   /*    */ "☂️"_Stz, // secure
   /*    */ "🌂"_Stz, // unsecure
   /*    */ "🏃‍♀️"_Stz, // run
   /*    */ "♟"_Stz, // move
   /*    */ "🐝"_Stz, // get
   /*    */ "🍿"_Stz, // pop
   /*    */ "🔍"_Stz, // find
   /*    */ "🦴"_Stz, // found
   /*    */ "🕳"_Stz, // not found
   /*    */ "💎"_Stz, // clarity
   /*    */ "💠"_Stz, // solidity
   /*    */ "🌀"_Stz, // confusion
   /*    */ "🎠"_Stz, // trigger
   /*    */ "🏖"_Stz, // bide
   /*    */ "🥁"_Stz, // end
   /*    */ "🎬"_Stz, // begin
   /*    */ "🕺"_Stz, // enter
   /*    */ "📡"_Stz, // connect
   /*    */ "🐾"_Stz, // trace
   /*    */ "🌗"_Stz, // phase
   /*    */ "🐿"_Stz, // seed
   /*    */ "🐽"_Stz, // check

   /*    */ "💈"_Stz, // job
   /*    */ "🔒"_Stz, // lock
   /*    */ "🛎"_Stz, // request
   /*    */ "🚥"_Stz, // mutex
   /*    */ "🎤"_Stz, // session
   /*    */ "🔌"_Stz, // socket
   /*    */ "❓"_Stz, // prompt
   /*    */ "📎"_Stz, // task
   /*    */ "〰️"_Stz, // thread

   /*    */ "🔸"_Stz, // cursor


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
