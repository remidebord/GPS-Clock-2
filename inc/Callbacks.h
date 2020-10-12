#ifndef __CALLBACKS_H
#define __CALLBACKS_H

/* includes ---------------------------------------------------------------- */
/* defines ----------------------------------------------------------------- */
/* structure --------------------------------------------------------------- */
/* functions --------------------------------------------------------------- */
void tickRTC(void);
void tickMCU(void);
void EncoderRightirqA(void);
void EncoderRightirqB(void);
void EncoderLeftirqA(void);
void EncoderLeftirqB(void);

#endif
