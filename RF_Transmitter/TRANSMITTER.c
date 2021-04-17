#include <TRANSMITTER.h>
#include <math.h>

#define SDI PIN_C4 //  (OUTPUT) PIC E GÖRE
#define SDO PIN_D3 //  (INPUT)
#define SCLK PIN_D2 // (OUTPUT)
#define nSEL PIN_D1 // (OUTPUT)
#define nIRQ PIN_D0 // (INPUT)
unsigned int8 tx_data[1]={0x41}; //0x41=A IN ASCII TABLE
unsigned int8 rx_buf[1];
unsigned int8 spi_readdummy(unsigned int8 data) // for recieving data from rf module we must send dummy data to the rf module and get the real data from rf module to mcu with SDO Pin
{
//this data is dummy (0x00) and we must send to the rf module (right after sending the adress) in order to get data from it to the mcu
   unsigned int8 i=0;
   output_low(SCLK);
  
   for(i=0;i<8;i++)
   {
      if(data&0x80) //if bit of data is 1 
      output_high(SDI);
      else //if bit of data is 0
      output_low(SDI);
      data<<=1;   
      output_high(SCLK); 
      if(SDO) // reading the actual data from rf module
         data|=0x01;
      else
         data&=0xfe;

      output_low(SCLK);
   }
   return data;
   
}
unsigned int8 spi_readaddr(unsigned int8 addr) // (reciever part) before recieving data we must send the adrres from mcu to rf module(RF Module will read)
{
   unsigned int8 i=0;
  // output_low(SCLK);
  // output_low(nSEL); // must be low during the addr write
   for(i=0;i<8;i++)
   {
      if(addr&0x80)
      output_high(SDI);
      else 
      output_low(SDI);
      addr<<=1;
      output_high(SCLK);
      delay_cycles(1);
      output_low(SCLK);
      
   }
   return addr;
   
}

 unsigned int8 spi_write(unsigned int8 addr, unsigned int8 data) //(transmitter part) writing adress and data from mcu to rf module FIFO
{
//make sure to use addr|0x80 in the argument of addr for writing mode
   unsigned int8 i=0;
   output_low(SCLK);
   output_low(nSEL); // must be low during the data read write
   for(i=0;i<8;i++)
   {
      if(addr&0x80)
      output_high(SDI);
      else 
      output_low(SDI);
      addr<<=1;
      output_high(SCLK);
      delay_cycles(1);
      output_low(SCLK);
      
   }
   for(i=0;i<8;i++)
   {
      if(data&0x80) //if bit of data is 1 
      output_high(SDI);
      else //if bit of data is 0
      output_low(SDI);
      data<<=1;   
      output_high(SCLK); 
      delay_cycles(1);
      output_low(SCLK);
   }
   output_high(SCLK);
   output_high(nSEL); // must be high after finish Writing addr and data
   return data;
                                                                  //CLEAR FIFO!!!!?????
}

void main()
{

   while(TRUE)
   {
      spý_write(0x7f|0x80,tx_data[1]) //0x41=A IN ASCII TABLE
   }

}
