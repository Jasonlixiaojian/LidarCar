#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <rthw.h>
#include <rtthread.h>
/* ���ڽ����¼���־ */
#define controlEventEaix4 (1 << 0)

void Eaix4Scaning(void);
void Eaix4stop(void);
void Eaix4Version(void);
void Eaix4Health(void);
/* �¼����ƿ� */
extern struct rt_event ControlEvent;;
#endif
