void SPI_Init(){
    TRISB6 = 0;     // set RB6 == SPI_CLK as output
    TRISB4 = 1;     // set RB4 == SPI_SDI as input
    TRISC7 = 0;     // set RC7 == SPI_SDO as output
    TRISC6 = 0;     // set RC6 == SPI_SS as output

    PIR1bits.SSP1IF = 0;        // Clear interrupt flag
    PIE1bits.SSP1IE = 0;        // Disable MSSP interrupt
    //INTCONbits.PEIE = 0;      // Disable periferal interrupts -> this breaks USB!!!

    SSPCON1bits.SSPEN = 1;      // enable serial port
    SSPCON1bits.CKP = 0;        // clock IDLE state is LOW
    SSPSTATbits.CKE = 1;        // Data transmitted from ACTIVE to IDLE clock
    SSPCON1bits.SSPM = 0x01;    // Master mode, clock = FOSC/64
    SSPSTATbits.SMP = 0;        // input sampled at middle of interval
}

void SPI_Write(char data){
    unsigned char TempVar;
    TempVar = SSP1BUF;              // Clear buffer by reading
    PIR1bits.SSP1IF = 0;            // Clear interrupt flag
    SSP1BUF = data;                 // write byte to SSP1BUF register
    while(!PIR1bits.SSP1IF);        // wait until bus cycle complete
    RC2 = 1;
}


unsigned char SPI_Read(void){
   SSPBUF = 0x00;                  // initiate bus cycle
   while ( !SSPSTATbits.BF );      // wait until cycle complete
   return ( SSPBUF );              // return with byte read 
}
 
