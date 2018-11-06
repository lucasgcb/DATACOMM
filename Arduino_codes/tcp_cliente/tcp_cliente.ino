#include <Ethernet.h>
#include <SPI.h>
#define PORT 51717

 
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(172,16,157,33); // IP Adress to our Server
int serverPort=PORT;
unsigned int idMaster = 0;
int sensorTemp = 0;
struct frame
{
  unsigned int id;
  unsigned char command;
  unsigned char size;
  unsigned char payload[16];
}dado;

EthernetClient client;
void setup() {

  Serial.begin(9600);
 
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
      ;
  }  
  Serial.print("Cliente iniciado em: ");
  Serial.println(Ethernet.localIP());
  
}
char buffer [16];
void loop()
{
  delay(1000);
  sensorTemp = analogRead(0);
  Serial.println("Connecting to Server ...");
 
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected!");
    dado.id = idMaster++;
    dado.command = 's';
    dado.size = sizeof(sensorTemp);
    sprintf(buffer, "%d", sensorTemp);
    for(int i=0;i<16;i++)
    {
      dado.payload[i] = (unsigned char) buffer[i];
    } 
    
    
    Serial.println("Sending Message ");
     Serial.println(sizeof(dado));

    
    client.write((const uint8_t*)&dado, sizeof(dado));

  }
  else {
    Serial.println("Connection failed.");
  }
  delay(2000);
  
  while(client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  Serial.println("");
  client.stop();    
  /*if (client.connected()) {
  
  }*/
  
  delay(5000);
    
}
