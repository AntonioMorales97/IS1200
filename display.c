

#include <stdint.h> //Integer types, example uint8_t unsigned integer 8 bits
#include "pic32mx.h"
#include "project.h"

/* Write data to the display panel via SPI controller.
	This information can be seen here: https://reference.digilentinc.com/chipkit_shield_basic_io_shield/refmanual
*/
uint8_t spi_send_get(uint8_t data) {
	while(!(SPI2STAT & 0x08)); //Wait for transmitter to be ready (Bit 3)
	SPI2BUF = data; //Send data to buffer
	while(!(SPI2STAT & 1)); //Wait for receive
	return SPI2BUF;
}
/*
Very lame function, but helps in some cases like
display init.
*/
void quick_wait (int c){
	int i;
  for(i = 0; i<c;i++);
}
/* Following commands does the initialization of the display.
	 The start-up sequence can be seen here: https://reference.digilentinc.com/chipkit_shield_basic_io_shield/refmanual
	 and register information here: http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf
	 and commands can be seen in pic32mx.h
*/
void display_init(void){

  //Initializes SPI Port 2
  SPI2CON = 0;
  SPI2BRG = 15; //8 Mhz
  SPI2STATCLR = 0x40; //Clear the Spirov bit, bit #6 in SPI2STAT
  SPI2CONSET = 0x8060; //Set the MSTEN, CKP and ON, bits 5, 6 and 15 in SPI2CON

  //Configure pins of PORTF and PORTG, and also TRISF and TRISG
  TRISFCLR = 0xFFFF; //Set everything to outputs
  TRISGCLR = 0xFFFF;
  PORTF = 0xFFFF; //this is a good start, everything is "off"
  PORTGSET = 0x200; //this also is a good start, reset-pin

  //Start sending init commands
  PORTFCLR = 0x10; //clear the data/cmd bit
  PORTFCLR = 0x40; //Turn on VDD
  quick_wait(10); //W8
  spi_send_get(0xAE); //Display off command
  //Set reset to low and then high
  PORTGCLR = 0x200;
  quick_wait(10);
  PORTGSET = 0x200;
  //Send commands for setting Charge pump and pre-charge period, enable charge
  spi_send_get(0x8D);
  spi_send_get(0x14);
  spi_send_get(0xD9);
  spi_send_get(0xF1);
  //Turn on vcc and wait
  PORTFCLR = 0x20; //bit 5
  quick_wait(1000000);
  /*Send the last commands to invert the diplay and to select config so that
  * the display memory will be non-interleaved
  */
  spi_send_get(0xA1);//remap column
  spi_send_get(0xC8);//remap the rows
  spi_send_get(0xDA);//set COM config
  spi_send_get(0x20);//left/right remap enabled

  spi_send_get(0xAF); //turn on the display
}

//Set a pixel to 1 or 0
void display_pixel (int x, int y, int on_off){
	if(x >= MAX_X || x < 0 || y >= MAX_Y || y < 0) //if it is out of range
		return;

	display[x][y]=on_off; //turn on/off
}

//Put everything to 0, works as a reset to remove old data from our matrix
void display_zero (void){
	int x,y;
	for (x = 0; x < MAX_X; x++)
		for (y = 0; y < MAX_Y; y++)
			display[x][y] = 0;
}

/*
Update display with all the data in our display matrix by sending it via the SPI to
the display memory (512 bytes, 4 pages, each page 128 bytes).
The SPI-commands and the display memory structure can be found in the previously source refered.
*/
void display_update (void){

	uint8_t data_buffer; //unsigned 8 bit integer
  int x,y;
	for (y = 0; y < MAX_Y / 8; y++){   // 32/8 = 4, which is number of page memory
		PORTFCLR = 0x10; //Clear bit 4 for start sending commands, command-mode

		spi_send_get(0x22); //Set page command
		spi_send_get(y); //page number (0, 1, 2, 3)
		//start at left column
		spi_send_get(0x00);
		spi_send_get(0x10);

		PORTFSET=0x10; //Set bit 4 for start sending data, data-mode

		//"Each byte in the memory page corresponds to an 8-pixel-high column on the display"
		// We most use binary-or so we don't change data_buffer for each row in the same column (in one page) before sending it to the display
		//This will construct one byte that corresponds one column in a page of the display accordinhly to our matrix.
		for (x = 0; x < MAX_X; x++){
			data_buffer = display[x][y * 8];
			data_buffer |= display[x][y * 8 + 1]<<1;
			data_buffer |= display[x][y * 8 + 2]<<2;
			data_buffer |= display[x][y * 8 + 3]<<3;
			data_buffer |= display[x][y * 8 + 4]<<4;
			data_buffer |= display[x][y * 8 + 5]<<5;
			data_buffer |= display[x][y * 8 + 6]<<6;
			data_buffer |= display[x][y * 8 + 7]<<7;
			spi_send_get (data_buffer); //Send data to display memory
		}
	}
}

/*
Displays (in our matrix) an object. For example a player or a block.
Parameters x and y decides the size in x-line and y-line. pixel_array is the
object being displayed.
*/
void display_object (int pos_x, int pos_y, int x, int y, const char *pixel_array){
	int i,j;
	for (i = 0; i < x; i++)
		for (j = 0; j < y; j++)
			if(*((pixel_array+i*y)+j) ) //See it like a matrix, displaying column after column
				display_pixel(pos_x + i, pos_y + j, 1);
}
