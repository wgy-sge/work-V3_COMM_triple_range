#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "include.h"

void net_port_config(void);
bool set_network(uint8 *mac,uint8 *ip,uint8 *sub,uint8 *gatewall);
void net_phyreset(void);
bool net_linkconfig(void);

#endif