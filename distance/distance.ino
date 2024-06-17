// Inclui a biblioteca NewPing para facilitar o uso do sensor ultrassônico
#include <NewPing.h>

// Define os pinos TRIGGER e ECHO do sensor ultrassônico
#define TRIGGER_PIN 3
#define ECHO_PIN 2

// Define a distância máxima suportada pelo sensor em centímetros
#define MAX_DISTANCE 400

// Cria um objeto NewPing para o sensor ultrassônico
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial com baud rate 9600
}

void loop() {
  delay(500); // Aguarda um curto período de tempo entre as medições

  // Realiza a medição da distância em centímetros
  unsigned int distance = sonar.ping_cm();

  // Verifica se a medição retornou um valor válido
  if (distance != 0) {
    // Imprime a distância medida no monitor serial
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    // Caso a medição não seja válida, imprime uma mensagem de erro
    Serial.println("Falha na medição!");
  }
}
