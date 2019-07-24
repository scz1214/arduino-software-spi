#define PIN_CS    2
#define PIN_CLK   3
#define PIN_MISO  4
#define PIN_MOSI  5

byte reg_data[]={0x1a,0x2b,0x3c,0x4d,0x5e,0x6f,0xff};
byte reg=0;
bool SPI_CE_FLAG=false;
bool SPI_READ=true;
byte spi_clk_c=0;
byte send_data=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_CS, INPUT);
  pinMode(PIN_CLK, INPUT);
  pinMode(PIN_MISO, OUTPUT);
  pinMode(PIN_MOSI, INPUT);
  Serial.begin (9600);
  attachInterrupt(digitalPinToInterrupt(PIN_CS), check_cs, CHANGE );
  attachInterrupt(digitalPinToInterrupt(PIN_CLK), check_clk, RISING  );
}
void check_cs()
{
  spi_clk_c=0;
  if(digitalRead(PIN_CLK))
  {
    SPI_CE_FLAG=false;
    reg=0;
  }
  else
  {
    SPI_CE_FLAG=true;
    SPI_READ=true;
    reg=0;
  }
}
void check_clk()
{
  if(!SPI_CE_FLAG) return;
  if(SPI_READ)
  {
    if(digitalRead(PIN_MOSI))
      reg|=(1<<(7-spi_clk_c));
    spi_clk_c++;
    if(spi_clk_c==8)
    {
      spi_clk_c=0;
      if(reg>5)
        send_data=0xff;
      else
        send_data=reg_data[reg];
      Serial.print("receive:");
      Serial.println(reg,HEX);
      Serial.print("Send:");
      Serial.println(send_data,HEX);
      SPI_READ=false;
    }
  }
  else
  {
    if((send_data & 0x80)==0x80)
      digitalWrite(PIN_MISO, 1);
    else
        digitalWrite(PIN_MISO, 0);
    send_data = (send_data << 1);
    spi_clk_c++;
    if(spi_clk_c==8)
    {
      Serial.println("Send Complete");
    }
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}
