/* Programa para obtenção de dados do Higrômetro */

// Define os pinos do Higrômetro
const int analogInPin0 = A0;


// Armazena na variável abaixo o valor obtido do Higrômetro (Tipo Inteiro)
int higroValor0 = 0;     


// Armazena o somatório de 30 amostras
long int higroSoma0 = 0;   


void setup() {
  // Inicializa a comunicação serial a 9600bps
  Serial.begin(9600); 
}

void loop() {
  
  for(int i = 0; i < 30; i++) // Lê 30 amostras em 3 segundos
  {
    higroSoma0 = higroSoma0 + analogRead(analogInPin0);  
    delay(100);  
  }
  higroValor0 = higroSoma0 / 30;  // Obtém o valor médio

  // Imprime os resultados no monitor serial:
  Serial.print("Higrômetro = " );                       
  Serial.print(higroValor0);      
  Serial.println(); 
  
  higroSoma0 = 0;  // Reseta as variáveis
  delay(4000);     // Delay 4s 
}
