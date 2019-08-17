#include <pic14regs.h>

#include "config.h"

void isr_ssp(void);
void isr_tx(void);
void isr_rc(void);
void isr_tmr2(void);
void isr_tmr1(void);
void isr_c1(void);
void isr_c2(void);
void isr_ad(void);
void isr_ee(void);
void isr_osf(void);
void isr_ccp1(void);
void isr_t0(void);
void isr_rab(void);

static void interrupt(void) __interrupt
{
#ifdef INT_SSP
    if (SSPIF) {
        isr_ssp();
        SSPIF = 0;
    }
#endif

#ifdef INT_TX
    if (TXIF) {
        isr_tx();
        TXIF = 0;
    }
#endif

#ifdef INT_RC
    if (RCIF) {
        isr_rc();
        RCIF = 0;
    }
#endif

#ifdef INT_TMR2
    if (TMR2IF) {
        isr_tmr2();
        TMR2IF = 0;
    }
#endif

#ifdef INT_TMR1
    if (TMR1IF) {
        isr_tmr1();
        TMR1IF = 0;
    }
#endif

#ifdef INT_C1
    if (C1IF) {
        isr_c1();
        C1IF = 0;
    }
#endif

#ifdef INT_C2
    if (C2IF) {
        isr_c2();
        C2IF = 0;
    }
#endif

#ifdef INT_AD
    if (ADIF) {
        isr_ad();
        ADIF = 0;
    }
#endif

#ifdef INT_EE
    if (EEIF) {
        isr_ee();
        EEIF = 0;
    }
#endif

#ifdef INT_OSF
    if (OSFIF) {
        isr_osf();
        OSFIF = 0;
    }
#endif

#ifdef INT_CCP1
    if (CCP1IF) {
        isr_ccp1();
        CCP1IF = 0;
    }
#endif

#ifdef INT_T0
    if (T0IF) {
        isr_t0();
        T0IF = 0;
    }
#endif

#ifdef INT_RAB
    if (RABIF) {
        isr_rab();
        RABIF = 0;
    }
#endif
}
