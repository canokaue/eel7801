/* Sistema de Irrigação Automático */

// Carrega bibliotecas para comunicação com o Ethernet Shield
#include <SPI.h>
#include <String.h>
#include <Ethernet.h>

// Define os parâmetros da Planta
#define higroValorDisparo0 600  // O valor de disparo do higrômetro, o valor mínimo é 0, siginifica que o solo está seco. Valor Máximo é 1024.
#define irrigaTempo0 10000  // Tempo de irrigação da planta em milisegundos. 35ms utiliza em torno de 70ml de água.


// Define os pinos do Higrômetro
const int analogInPin0 = A0;


// Armazena na variável abaixo o valor obtido do Higrômetro (Tipo Inteiro)
int higroValor0 = 0;     

// Armazena o somatório de 30 amostras
long int higroSoma0 = 0; 


// Define o pino do Relê
const int RelePin =  8;      // Pino 13 conectado ao Relê

// Define os parâmetros do Ethernet Shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x0B, 0x63 }; // Mac Adress do Arduino
//byte ip[] = { 200, 135, 84, 25 }; // Endereço IP do Arduino
EthernetServer servidor(8080); // Porta de conexâo com o Servidor HTTP

// String que receberá as informações do servidor HTTP
String lerString = String(90);

void setup() 
{ 
  Ethernet.begin(mac);      // Inicializa o Ethernet Shield 
  //servidor.begin();				// Inicia o Servidor HTTP
  pinMode(RelePin, OUTPUT);     // Seta o pino do Relê como saída
  delay(500);  
  Serial.begin(9600); 			// Inicializa a comunicação serial a 9600bps						
  Serial.println(Ethernet.localIP()); //teste
} 

void loop() 
{
  EthernetClient cliente = servidor.available();	// Cria uma conexão com um cliente
  
  if (cliente) //== true)
  {
    while (cliente.connected())
    {
      if (cliente.available())
      {
        char tempHTTP = cliente.read();	// Lê os caracteres exibidos
        
        if (lerString.length() < 100)
        {
          lerString = lerString + tempHTTP;
        }
        
        if (tempHTTP == '\n')
        {

         if(lerString.indexOf("/?coletar") > 0) 
        {
          higroAmostra();
        }
          
        if(lerString.indexOf("/?irrigar") > 0) 
        {
          higroAmostra();
        
          if(higroValor0 > higroValorDisparo0)
          {
            irriga(irrigaTempo0);
            //return;
          }
        }

        if(lerString.indexOf("/?auto") > 0) 
        {
          higroAmostra();
        
          if(higroValor0 > higroValorDisparo0)
          {
            irriga(irrigaTempo0);
            return;
          }
        }

        //Serial.println(lerString);
          
		    cliente.println("http/1.1 200 ok");
        cliente.println("content-type: text/html");
        cliente.println();
          
		    cliente.println("<!doctype html><html>");
        cliente.println("<center><head><title>'Irrigador Automatico'</title></head>");
        cliente.println("<input type=button value='Irrigar' onmousedown=location.href='/?irrigar'>");
        cliente.println("<input type=button value='Autonomo' onmousedown=location.href='/?auto'>");
        cliente.println("<input type=button value='Coletar' onmousedown=location.href='/?coletar'><br/><br/>");

        cliente.println();
        cliente.println("Higrometro :");
        cliente.println(higroValor0);
        cliente.println();

                
        cliente.println("</body></html>");
          
		  //delay(1000);

     lerString = "";  // Limpa o string para a próxima leitura
          
		  cliente.stop();	// Para Cliente
     
          
        }
      }
    }
  }
  
      
}

void higroAmostra() // Lê o valor do Higrômetro
{
  for(int i = 0; i < 30; i++) // Lê 30 amostras em 3 segundos
  {
    higroSoma0 = higroSoma0 + analogRead(analogInPin0);  
    //delay(100);  
  }
  higroValor0 = higroSoma0 / 30;  // Obtêm o valor médio    

  // Imprime os resultados:
  Serial.print("Higrômetro = " );                       
  Serial.print(higroValor0);      
  Serial.println(); 

    
  higroSoma0 = 0;  // Reseta as variáveis
  //delay(4000);     // Delay 4s 
}


void irriga(int irrigaTempo)
{
  delay(500);
  digitalWrite(RelePin, HIGH);
  delay(irrigaTempo);
  digitalWrite(RelePin, LOW);
  delay(500);
}
