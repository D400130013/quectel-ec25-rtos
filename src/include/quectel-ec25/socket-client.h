/*

 * socket-client.h
 *
 *  Created on: Nov 10, 2022
 *      Author: janoko
 */

#ifndef QUECTEL_EC25_SOCKET_CLIENT_H_
#define QUECTEL_EC25_SOCKET_CLIENT_H_

#include <quectel-ec25/conf.h>
#if QTEL_EN_FEATURE_SOCKET

#include <quectel-ec25/types.h>

#define SIM_SOCK_UDP    0
#define SIM_SOCK_TCPIP  1

#define SIM_SOCK_EVENT_ON_OPENED        0x01
#define SIM_SOCK_EVENT_ON_OPENING_ERROR 0x02
#define SIM_SOCK_EVENT_ON_RECEIVED      0x04
#define SIM_SOCK_EVENT_ON_CLOSED        0x08

enum {
  SIM_SOCK_CLIENT_STATE_CLOSE,
  SIM_SOCK_CLIENT_STATE_WAIT_NETOPEN,
  SIM_SOCK_CLIENT_STATE_OPENING,
  SIM_SOCK_CLIENT_STATE_OPEN_PENDING,
  SIM_SOCK_CLIENT_STATE_OPEN,
};


typedef struct {
  struct QTEL_Socket_HandlerTypeDef *socketManager;
  uint8_t     state;
  uint8_t     events;               // Events flag
  int8_t      linkNum;
  uint8_t     type;                 // SIM_SOCK_UDP or SIM_SOCK_TCPIP

  // configuration
  struct {
    uint32_t timeout;
    uint8_t  autoReconnect;
    uint16_t reconnectingDelay;
  } config;

  // tick register for delay and timeout
  struct {
    uint32_t reconnDelay;
    uint32_t connecting;
  } tick;

  // server
  char     host[64];
  uint16_t port;

  void *buffer;

  // listener
  struct {
    void (*onConnecting)(void);
    void (*onConnected)(void);
    void (*onConnectError)(void);
    void (*onClosed)(void);
    void (*onReceived)(void *buffer);
  } listeners;
} QTEL_SocketClient_t;


// SOCKET
QTEL_Status_t  SIM_SockClient_Init(QTEL_SocketClient_t*, const char *host, uint16_t port, void *buffer);
QTEL_Status_t  SIM_SockClient_CheckEvents(QTEL_SocketClient_t*);
QTEL_Status_t  SIM_SockClient_OnNetOpened(QTEL_SocketClient_t*);
QTEL_Status_t  SIM_SockClient_Loop(QTEL_SocketClient_t*);
void          SIM_SockClient_SetBuffer(QTEL_SocketClient_t*, void *buffer);
QTEL_Status_t  SIM_SockClient_Open(QTEL_SocketClient_t*, void*);
QTEL_Status_t  SIM_SockClient_Close(QTEL_SocketClient_t*);
uint16_t      SIM_SockClient_SendData(QTEL_SocketClient_t*, uint8_t *data, uint16_t length);


#endif /* QTEL_EN_FEATURE_SOCKET */
#endif /* QUECTEL_EC25_SOCKET_CLIENT_H_ */