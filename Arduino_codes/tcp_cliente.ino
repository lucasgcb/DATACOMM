#include <Crypto.h>
#include <AES.h>

#include <Ethernet.h>
#include <SPI.h>

#define TAMANHO_PAYLOAD 16
#define PORTA 8888
 
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(172,16,157,33); // IP Adress to our Server
int serverPort=PORTA;
unsigned int idMaster = 0;
int sensorTemp = 0;

typedef struct frame
{
  unsigned int id;
  unsigned char command;
  unsigned char size;
  unsigned char payload[TAMANHO_PAYLOAD];
}pacoteFrame;

union payload_buffer{
  int buff;
  unsigned char payload[TAMANHO_PAYLOAD];
};
///////Crypto
byte bufferCrypto[TAMANHO_PAYLOAD];
byte chave[TAMANHO_PAYLOAD] = {'k','k','e','a','e','m','e','n', 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
AESTiny128 aes128;
void testCipher(BlockCipher *cipher, pacoteFrame *frame)
{
    Serial.print("///////"); Serial.print("Encryption in progress:");
    Serial.print(frame->id);
    Serial.println(" Encryption ... ");
    cipher->setKey(chave, cipher->keySize());
    cipher->encryptBlock(bufferCrypto, frame->payload);
    for(uint8_t i=0;i<TAMANHO_PAYLOAD;i++)
    {
      Serial.print(bufferCrypto[i]);
      frame->payload[i] = (unsigned char) bufferCrypto[i];
    }
    Serial.println("///////");
}
///////
pacoteFrame temp;
pacoteFrame dado;

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
int delayzera = 1000;
void loop()
{
  delay(delayzera);
  sensorTemp = analogRead(0);
  Serial.println("Connecting to Server ...");
  union payload_buffer pBuffer;
  
  memset(&dado, 0, sizeof(dado));
  memset(&buffer, 0, sizeof(buffer));
    
  if (client.connect(serverIP, serverPort)) 
  {
    Serial.println("Connected!");
    dado.id = idMaster++;
    dado.command = 's';
    sprintf(buffer, "%d", sensorTemp);
    dado.size = sizeof(sensorTemp);
    Serial.println("Tamanho payload:"); Serial.println(sizeof(sensorTemp));
    for(int i=0;i<16;i++)
    {
      dado.payload[i] = (unsigned char) buffer[i];
    } 
    Serial.println("Sending Message with the total size of"); Serial.println(sizeof(dado)); Serial.println("Payload:");
     /*Serial.println("Original:"); 
    for(uint8_t i=0;i<TAMANHO_PAYLOAD;i++)
    {
      Serial.print(dado.payload[i]);
    }*/
    ////////////Crypto
    testCipher(&aes128, &dado);
    
    client.write((const uint8_t*)&dado, sizeof(dado));
    ////////////
    /*Serial.println("Cifrado:");
    for(uint8_t i=0;i<TAMANHO_PAYLOAD;i++)
    {
      Serial.print(dado.payload[i]);
    }
    Serial.println("-----------");*/
  }
    else 
    {
      Serial.println("Connection failed.");
    }
  delay(delayzera);
  while(client.available()) 
  {
    Serial.println("client available");
    memset(&temp, 0, sizeof(temp));
    temp.id = client.read();
    /*temp.id = temp.id >> 8 | client.read(); //shift 8 little endian target host
    temp.command = client.read();
    temp.size = client.read();
    for(uint8_t i=0;i<TAMANHO_PAYLOAD+1;i++)
    {
      temp.payload[i] = client.read();
    }*/
    Serial.println("///////");
    Serial.println("Server Response:");
    Serial.print(temp.id);
    Serial.println("///////"); 
    /*Serial.println("id:");   Serial.print(temp.id); Serial.println("\n command:");Serial.print(temp.command);Serial.println("\n size:"); Serial.print(temp.size); Serial.println("\n payload:");
    for(uint8_t i=0;i<TAMANHO_PAYLOAD;i++)
    {
      Serial.print(temp.payload[i]);
    }*/
    /// User Config Updater
    /*char batatao[TAMANHO_PAYLOAD];
    if(temp.command=='a')
    {
      for(uint8_t i=0;i<TAMANHO_PAYLOAD;i++)
      {
        batatao[i]=temp.payload[i];
      }
      
      Serial.println(batatao);
      delayzera=atoi(batatao);
      
      Serial.println("NOVO DELAY:"); Serial.println(delayzera);
    }*/
  }
  
  Serial.println("");
  client.stop();    
  /*if (client.connected()) {
  
  }*/
  
  delay(5000);
    
}
