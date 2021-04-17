#include <RECIEVERREAL.h>
#include <math.h>

unsigned char ItStatus1,ItStatus2;
unsigned char rx_buf[9];



void main()
{
   unsigned char i;
   set_tris_d(0b00000001);
   set_tris_b(0b00000000);
   set_tris_c(0b00010000);
   SI4432_RX_init_parameter();
   spi_writex(0x0e|0x80, 0x00); // TX0_RX0 Antenna is not in Tx/Rx mode 
   while(TRUE)
   {    
      delay_ms(100);
      rx_data();
      while(input(nIRQ));
      if(!input(nIRQ))
      { 
         for(i=0;i<5;i++)
         {
            output_high(LEDB5);
            delay_ms(200);
            output_low(LEDB5);
            delay_ms(200);
         }

      ItStatus1 = spi_readx(0x03); // read status, clear interrupt
      ItStatus2 = spi_readx(0x04);
      output_low(SCLK);
      output_low(nSEL); // must be low during the addr write
      spi_R(0x7f);
      for( i=0;i<9;i++)
         {
            rx_buf[i]=spi_R(0x00);
         }
      output_high(nSEL); 
      spi_writex(0x07, SI4432_PWRSTATE_READY); // Exit Rx mode after all the data read from the FiFo
           
        // chksum = 0;
        // for(i=0;i<3;i++) // calculate the checksum for the received data
         //{chksum += rx_buf[i];}
         //if(( chksum== rx_buf[3] )&&( rx_buf[0] == 0x41 ))
//!       if(rx_buf[0] == 0x41 )
//!         {
          
               output_high(LEDB5);
               delay_ms(30);
               for(i=0;i<9;i++)
               {
               printf("data is= %c \n ",rx_buf[i]);// data verified OK
               }

         //   }
        
//!         else
//!         {
//!           printf("Data Verified error, then restart to Rx");
//!             for(i=0;i<7;i++)
//!        {
//!           output_high(LEDB5);
//!           delay_ms(50);
//!           output_low(LEDB5);
//!            delay_ms(50);
//!         }
//!            rx_data(); // Data Verified error, then restart to Rx
//!          }
      
      }
   }

}
 
 void SI4432_RX_init_parameter(void)
{
   unsigned char fb=0x53,fc0=0x64,fc1=0x00; // //434 mhz
   ItStatus1 = spi_readx(0x03); // read status, clear interrupt
   ItStatus2 = spi_readx(0x04);
   spi_writex(0x06, 0x00); // no wakeup up, lbd,
   spi_writex(0x07, SI4432_PWRSTATE_READY); // disable lbd, wakeup timer, use internal 32768,xton = 1; in ready mode
   spi_writex(0x09, 0x7f); // c = 12.5p
   spi_writex(0x0a, 0x05);
   spi_writex(0x0b, 0xf4); // gpio0 rx data output
   spi_writex(0x0c, 0xfd); // gpio vdd
   spi_writex(0x0d, 0xfd); // gpio vdd
   spi_writex(0x0e, 0x00); // gpio 0, 1,2 NO OTHER FUNCTION.
   spi_writex(0x70, 0x20); // disable manchest
   spi_writex(0x1d, 0x00); // enable afc
   spi_writex(0x1c, 0x1d); // RATE_24K: // 2.4k
   spi_writex(0x20,0x41);//0x20 calculate from the datasheet= 500*(1+2*down3_bypass)/(2^ndec*RB*(1+enmanch))
   spi_writex(0x21, 0x60); // 0x21 , rxosr[10--8] = 0; stalltr = (default), ccoff[19:16] = 0;
   spi_writex(0x22, 0x27); // 0x22 ncoff =5033 = 0x13a9
   spi_writex(0x23, 0x52); // 0x23
   spi_writex(0x24, 0x00); // 0x24
   spi_writex(0x25, 0x06); // 0x25
   spi_writex(0x2a, 0x1e);
   spi_writex(0x30, 0x8c); // enable packet handler, msb first, enable crc,
// 0x31 only readable
   spi_writex(0x32, 0xff); // 0x32address enable for headere byte 0, 1,2,3, receive header check for byte 0, 1,2,3
   spi_writex(0x33, 0x42); // header 3, 2, 1,0 used for head length, fixed packet length,synchronize word length 3, 2,
   spi_writex(0x34, 64); // 64 nibble = 32byte preamble
   spi_writex(0x35, 0x20); //0x35 need to detect 20bit preamble
   spi_writex(0x36, 0x2d); // synchronize word
   spi_writex(0x37, 0xd4);
   spi_writex(0x38, 0x00);
   spi_writex(0x39, 0x00);
   spi_writex(0x3f, 's'); // check hearder
   spi_writex(0x40, 'o');
   spi_writex(0x41, 'n');
   spi_writex(0x42, 'g');
   spi_writex(0x43, 0xff); // all the bit to be checked
   spi_writex(0x44, 0xff); // all the bit to be checked
   spi_writex(0x45, 0xff); // all the bit to be checked
   spi_writex(0x46, 0xff); // all the bit to be checked
// 0x56 ---------0x6c ??????????????????????????
   spi_writex(0x79, 0x0); // no hopping
   spi_writex(0x7a, 0x0); // no hopping
   spi_writex(0x71, 0x22); // Gfsk, fd[8] =0, no invert for Tx/Rx data, fifo mode, txclk -->gpio
   spi_writex(0x73, 0x0);
   spi_writex(0x74, 0x0); // no offset
//band 434:
   spi_writex(0x75, fb); // hbsel = 0, sbsel =1 ???, fb = 19
   spi_writex(0x76, fc0); // 25600= 0x6400 for 434Mhz
   spi_writex(0x77, fc1);
}

void rx_data()
{  
   spi_writex(0x07|0x80, SI4432_PWRSTATE_READY); // enter Ready mode
   delay_ms(5); // stablize the OSC; not needed if OSC is on
   spi_writex(0x0e|0x80, 0x02);//TX0_RX1 antenna switch = Rx mode
   spi_writex(0x08, 0x03); //clear Tx/Rx fifo
   spi_writex(0x08, 0x00); //clear Tx/Rx fifo  WE MUST SET TO THE DEFAUT VALUES=0
   spi_writex(0x07,SI4432_PWRSTATE_RX ); // enter Rx mode //AUTOMATICALLY CLEARED WHEN A VALID PACKET RECýEVED
   spi_writex(0x05, SI4432_Rx_packet_received_interrupt); // interrupt for packet received
   //when we write interrupt in 0x05 register we must read status registers(03h,04h) in order to enable nIRQ=0 pin.
   //after that we must read status registers for disable the interrupts.
   ItStatus1 = spi_readx(0x03); // read status, clear interrupt
   ItStatus2 = spi_readx(0x04);
}

unsigned char spi_writex(unsigned char addr , unsigned char data )
{  
   unsigned char i;
   //make sure to use addr|0x80 in the argument of addr for writing mode
   addr|=0x80;
   output_low(SCLK);
   output_low(nSEL); // must be low during the data read write
   for(i=0;i<8;i++)
   {
      if(addr&0x80)
      output_high(SDO);
      else 
      output_low(SDO);
      addr<<=1;
      output_high(SCLK);
      delay_cycles(1);
      output_low(SCLK);   
   } 
   for(i=0;i<8;i++)
   {
      if(data&0x80) //if bit of data is 1 
      output_high(SDO);
      else //if bit of data is 0
      output_low(SDO);
      data<<=1;   
      output_high(SCLK); 
      delay_cycles(1);
      output_low(SCLK);
   }
   output_high(nSEL); // must be high after finish Writing addr and data
   output_high(SCLK);
   return data;
}
unsigned char spi_readx(unsigned char addr)
{
   unsigned char i;
   output_low(SCLK);
   output_low(nSEL); // must be low during the addr write
   for(i=0;i<8;i++)
   {
      if(addr&0x80)
      output_high(SDO);
      else 
      output_low(SDO);
      addr<<=1;
      output_high(SCLK);
      delay_cycles(1);
      output_low(SCLK);  
   }
      output_low(SDO);
      unsigned data;
   for(i=0;i<8;i++)
   {             
//!      if(data&0x80)
//!      output_high(SDO);
//!      else 
//!      output_low(SDO);
      data <<= 1;
      output_high(SCLK);  
      if(input(SDI)) // reading the actual data from rf module
        { data|=0x01;}
//!   else
//!     { data&=0xfe;}
      //data<<=1; //else kullanmazsak bunu alta da yazabiliriz.
      output_low(SCLK);
   }
      output_high(nSEL); 
      output_high(SCLK);
   return data;
}

unsigned char spi_R(unsigned char data)
{  
   unsigned char i;
   for(i=0;i<8;i++)
   {
  
      if(data&0x80) //if bit of data is 1 
      output_high(SDO);
      else //if bit of data is 0
      output_low(SDO);
      data<<=1;   
      output_high(SCLK); 
      if(input(SDI))
      data|=0x01;
      else
      data&=0xfe;
      output_low(SCLK);
   }
   return data;
}

