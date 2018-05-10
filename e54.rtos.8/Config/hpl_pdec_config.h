/* Auto-generated config file hpl_pdec_config.h */
#ifndef HPL_PDEC_CONFIG_H
#define HPL_PDEC_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

#ifndef CONF_PDEC_ENABLE
#define CONF_PDEC_ENABLE 1
#endif

// <h> Basic Configuration

// <q> Phase A Invert Enable
// <i> If enabled, Phase A active level is inverted
// <id> pdec_phasea_invert
#ifndef CONF_PDEC_PHASEA_INVERT
#define CONF_PDEC_PHASEA_INVERT 0x0
#endif

// <q> Phase B Invert Enable
// <i> If enabled, Phase B active level is inverted
// <id> pdec_phaseb_invert
#ifndef CONF_PDEC_PHASEB_INVERT
#define CONF_PDEC_PHASEB_INVERT 0x0
#endif

// <q> Index Invert Enable
// <i> If enabled, Index active level is inverted
// <id> pdec_index_invert
#ifndef CONF_PDEC_INDEX_INVERT
#define CONF_PDEC_INDEX_INVERT 0x0
#endif

// <q> Phase A and Phase B Swap
// <i> If enabled, Phase A and Phase B are swapped
// <id> pdec_phase_swap
#ifndef CONF_PDEC_PHASE_SWAP
#define CONF_PDEC_PHASE_SWAP 0x0
#endif

// <o> PDEC Configuration
// <0x0=>Quadrature decoder direction (X4)
// <0x1=>Secure Quadrature decoder direction (X4S)
// <0x2=>Decoder direction (X2)
// <0x3=>Secure decoder direction (X2S)
// <0x4=>Auto correction mode (AUTOC)
// <i> These bits define the the PDEC configuration
// <id> pdec_config
#ifndef CONF_PDEC_CONFIG
#define CONF_PDEC_CONFIG 0x4
#endif

// <o> Maximum Consecutive Missing Pulses <0-15>
// <i> These bits define the threshold for the maximum consecutive missing pulses in AUTOC mode
// <id> pdec_fliter
#ifndef CONF_PDEC_FILTER
#define CONF_PDEC_FILTER 0x0
#endif

// <q> Run in standby
// <i> Indicates whether the PDEC will continue running in standby sleep mode or not (RUNSTDBY)
// <id> pdec_runstdby
#ifndef CONF_PDEC_RUNSTDBY
#define CONF_PDEC_RUNSTDBY 0
#endif

// <q> Debug Run
// <i> If enabled, the PDEC is running if the CPU is halted by an external debugger (DBGRUN)
// <id> pdec_dbgrun
#ifndef CONF_PDEC_DBGRUN
#define CONF_PDEC_DBGRUN 0
#endif

// <o> Prescaler Value
// <0x00=>No division
// <0x01=>Divide by 2
// <0x02=>Divide by 4
// <0x03=>Divide by 8
// <0x04=>Divide by 16
// <0x05=>Divide by 32
// <0x06=>Divide by 64
// <0x07=>Divide by 128
// <0x08=>Divide by 256
// <0x09=>Divide by 512
// <0x0A=>Divide by 1024
// <id> pdec_prescaler
#ifndef CONF_PDEC_PRESCALER
#define CONF_PDEC_PRESCALER 0x0
#endif

// </h>

// <h> Interrupt Configuration

// <q> Position Changed
// <i> Indicates whether to enable Velocity interrupt
// <id> pdec_int_vel
#ifndef CONF_PDEC_INT_VEL
#define CONF_PDEC_INT_VEL 1
#endif

// <q> Direction Changed
// <i> Indicates whether to enable Direction Change interrupt
// <id> pdec_int_dir
#ifndef CONF_PDEC_INT_DIR
#define CONF_PDEC_INT_DIR 0
#endif

// <q> Overflow/Underflow
// <i> Indicates whether to enable Overflow/Underflow interrupt
// <id> pdec_int_overflow
#ifndef CONF_PDEC_INT_OVERFLOW
#define CONF_PDEC_INT_OVERFLOW 0
#endif

// <q> Error
// <i> Indicates whether to enable Error interrupt
// <id> pdec_int_error
#ifndef CONF_PDEC_INT_ERROR
#define CONF_PDEC_INT_ERROR 0
#endif

// </h>

// <e> Filter Configuration
// <id> pdec_filter_settings
#ifndef CONF_PDEC_FILTER_CONFIG
#define CONF_PDEC_FILTER_CONFIG 0
#endif

// <o> Minimum Duration <0-255>
// <i> Define the minimum duration for which the input signals has to be valid, the minimum duration must be FILTER * GCLK_PDEC
// <id> pdec_min_duration
#ifndef CONF_PDEC_DURATION
#define CONF_PDEC_DURATION 0
#endif

// </e>

// <e> Event Control
// <id> pdec_event_settings
#ifndef CONF_PDEC_EVENT_CONFIG
#define CONF_PDEC_EVENT_CONFIG 0
#endif

// <q> Phase A Match Event Out
// <i> Enables event output on Phase A every match
// <id> pdec_pha_matcheo
#ifndef CONF_PDEC_PHA_MATCHEO
#define CONF_PDEC_PHA_MATCHEO 0
#endif

// <q> Phase B Match Event Out
// <i> Enables event output on Phase B every match
// <id> pdec_phb_matcheo
#ifndef CONF_PDEC_PHB_MATCHEO
#define CONF_PDEC_PHB_MATCHEO 0
#endif

// <q> Position Chagned Event Out
// <i> Enables event output on each change of the qualified PDEC phases
// <id> pdec_velocityeo
#ifndef CONF_PDEC_VELOCITYEO
#define CONF_PDEC_VELOCITYEO 0
#endif

// <q> Direction Changed Event Out
// <i> Enables event output on direction change
// <id> pdec_direo
#ifndef CONF_PDEC_DIREO
#define CONF_PDEC_DIREO 0
#endif

// <q> Overflow/Underflow Event Out
// <i> Enables event output on position counter overflows/underflows
// <id> pdec_overflow
#ifndef CONF_PDEC_OVERFLOWEO
#define CONF_PDEC_OVERFLOWEO 0
#endif

// <q> Error Event Out
// <i> Enables event output on errors occured
// <id> pdec_erroreo
#ifndef CONF_PDEC_ERROREO
#define CONF_PDEC_ERROREO 0
#endif

// </e>

// <<< end of configuration section >>>

#endif // HPL_PDEC_CONFIG_H
