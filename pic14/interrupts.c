#include <pic14regs.h>

#include "config.h"

#ifdef INT_SSP
void isr_ssp(void);
#endif

#ifdef INT_TX
void isr_tx(void);
#endif

#ifdef INT_RC
void isr_rc(void);
#endif

#ifdef INT_TMR2
void isr_tmr2(void);
#endif

#ifdef INT_TMR1
void isr_tmr1(void);
#endif

#ifdef INT_C1
void isr_c1(void);
#endif

#ifdef INT_C2
void isr_c2(void);
#endif

#ifdef INT_AD
void isr_ad(void);
#endif

#ifdef INT_EE
void isr_ee(void);
#endif

#ifdef INT_OSF
void isr_osf(void);
#endif

#ifdef INT_CCP1
void isr_ccp1(void);
#endif

#ifdef INT_T0
void isr_t0(void);
#endif

#ifdef INT_RAB
void isr_rab(void);
#endif

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
