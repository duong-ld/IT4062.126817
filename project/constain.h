#ifndef _CONSTAIN_H_
#define _CONSTAIN_H_

enum {
  // server message status
  SUCCESS,
  FAILURE,
  // state of client
  NOT_AUTH,
  AUTH,
  IN_GAME,
  // message type and also user choose function
  LOGIN = 1,
  REGISTER = 2,
  LOGOUT = 3,
  JOIN_GAME = 4,
  EXIT = 5,
};

#endif  // _CONSTAIN_H_