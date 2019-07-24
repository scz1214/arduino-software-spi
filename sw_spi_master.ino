#define PIN_CS    2
#define PIN_CLK   3
#define PIN_MISO  4
#define PIN_MOSI  5

byte reg=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_MISO, INPUT);
  pinMode(PIN_MOSI, OUTPUT);
  Serial.begin (9600);
  swspi_init();
}
void swspi_init()
{
  digitalWrite(PIN_CS, 1);  
  digitalWrite(PIN_CLK, 0);
}
void sw_spi_write(byte data)
{
  byte i;
  byte spi_data=data;
  for(i=0;i<8;i++)
  {
    digitalWrite(PIN_CLK, 0);
    digitalWrite(PIN_MOSI, 0);
    if((spi_data & 0x80) == 0x80)
      digitalWrite(PIN_MOSI, 1);
    delay(10);
    digitalWrite(PIN_CLK, 1);
    delay(10);
    spi_data =(spi_data << 1);
    delay(10);
  }
}

unsigned char spi_read_byte()
{
  unsigned char data=0x00;
  unsigned char i=0;
  unsigned char temp=0;
  unsigned char sck_io=0;
  int cs,sck;
  digitalWrite(PIN_CLK, 0);
  digitalWrite(PIN_MOSI, 0);  
  for(i=0;i<8;i++)
  { 
    digitalWrite(PIN_CLK, 1);
    delay(10);
    if(digitalRead(PIN_MISO))
      data|=(1<<(7-i));
    digitalWrite(PIN_CLK, 0);
    delay(10);    
  }
  digitalWrite(PIN_CLK, 0);
  return data;
}
void loop() {
  // put your main code here, to run repeatedly:
  
  delay(1000);
  digitalWrite(PIN_CS, 0);
  sw_spi_write(reg);
  Serial.print("reg:");
  Serial.print(reg);
  Serial.print(" data:");
  Serial.println(spi_read_byte(),HEX);
  reg++;
  if(reg==6) reg=0;
  digitalWrite(PIN_CS, 1);
  delay(1000);
}
