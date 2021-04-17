#include <18LF46K22.h>
#device ADC=16
#FUSES NOMCLR
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
unsigned char spi_R(unsigned char data);
void SI4432_RX_init_parameter(void);
void rx_data();
unsigned char spi_writex(unsigned char addr, unsigned char data);
unsigned char spi_readx(unsigned char addr );
#define SI4432_PWRSTATE_READY 01 // Si4432 ready mode define
#define SI4432_PWRSTATE_TX 0x09 // Si4432 Tx mode define
#define SI4432_PWRSTATE_RX 05 // Si4432 Rx mode define
#define SI4432_PACKET_SENT_INTERRUPT 04 // Si4432 packet sent interrupt define
#define SI4432_Rx_packet_received_interrupt 0x02 // Si4432 packet received interrupt define

#define TX1_RX0 spi_writex(0x0e|0x80, 0x01) // Antenna switch to tx mode
#define TX0_RX1 spi_writex(0x0e|0x80, 0x02) // Antenna switch to Rx mode
#define TX0_RX0 spi_writex(0x0e|0x80, 0x00) // Antenna is not in Tx/Rx mode

#define LEDB5 PIN_B5 //(output)
#define SDO PIN_D3 //  (OUTPUT) PIC E GÖRE
#define SDI PIN_C4 //  (INPUT)
#define SCLK PIN_D2 // (OUTPUT)
#define nSEL PIN_D1 // (OUTPUT)
#define nIRQ PIN_D0 // (INPUT)
#use delay(clock=64MHz,crystal=16MHz)
#use rs232(baud=9600,parity=N,xmit=PIN_A1,rcv=PIN_A2,bits=8,stream=PORT1)

