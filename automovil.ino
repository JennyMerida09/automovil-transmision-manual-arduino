const int pinAcelerador = A0;  // Potenciómetro del acelerador
const int pinFreno = A1;       // Potenciómetro del freno
const int pinEmbrague = A2;    // Potenciómetro del embrague

// Pines de los Botones de Marcha
const int botonNeutro = 7;     
const int boton1 = 8;          
const int boton2 = 9;          
const int boton3 = 12;          

// Pines de Salida al Motor (L298N)
const int motor1PWM = 11;
const int motor2PWM = 10;// Velocidad 
const int motorDir1 = 3;
const int motorDir2 = 4;
const int motorDir3 = 5;
const int motorDir4 = 6;// Dirección 

float multiplicadorMarcha = 0.0; 
float marchaActual = 0.0;        

void setup() {
  pinMode(pinAcelerador, INPUT);
  pinMode(pinFreno, INPUT);
  pinMode(pinEmbrague, INPUT);
  
  pinMode(botonNeutro, INPUT_PULLUP);
  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
  pinMode(boton3, INPUT_PULLUP);

  pinMode(motor1PWM, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
  pinMode(motorDir1, OUTPUT);
  pinMode(motorDir2, OUTPUT);
  pinMode(motorDir3, OUTPUT);
  pinMode(motorDir4, OUTPUT);

  
  digitalWrite(motorDir1, HIGH); 
  digitalWrite(motorDir2, LOW);
  digitalWrite(motorDir3, HIGH);
  digitalWrite(motorDir4, LOW);

  Serial.begin(9600);
}

void loop() {
  // 1. LÓGICA DE EMBRAGUE Y MARCHAS
  int valorEmbrague = analogRead(pinEmbrague);

  if (valorEmbrague > 800) { // Si el embrague está pisado
    if (digitalRead(botonNeutro) == LOW) {
      marchaActual = 0.0;
    }
    else if (digitalRead(boton1) == LOW) {
      marchaActual = 0.4; // 1ra
    }
    else if (digitalRead(boton2) == LOW) {
      marchaActual = 0.7; // 2da
    }
    else if (digitalRead(boton3) == LOW) {
      marchaActual = 1.0; // 3ra
    }
  }
  multiplicadorMarcha = marchaActual;

  // 2. CÁLCULO DE ACELERACIÓN Y MARCHA
  int valorPedal = analogRead(pinAcelerador);
  float velocidadAcelerador = map(valorPedal, 0, 1023, 0, 255);
  float velocidadFinal = velocidadAcelerador * multiplicadorMarcha;

  // 3. LÓGICA DE FRENO
  int valorFreno = analogRead(pinFreno);
  
  // Si el freno es mayor a 100, reducimos o cortamos la velocidad
  if (valorFreno > 100) {
    // Opcional: Freno proporcional (cuanto más pisas, más frena)
    int reduccionFreno = map(valorFreno, 0, 1023, 0, 255);
    velocidadFinal = velocidadFinal - reduccionFreno;
    
    // Si la velocidad baja de 0, la dejamos en 0
    if (velocidadFinal < 0) {
      velocidadFinal = 0;
    }
  }

  // 4. MANDAMOS LA VELOCIDAD FINAL AL MOTOR
  int velocidadInt = (int)velocidadFinal;
  analogWrite(motor1PWM, velocidadInt);
  analogWrite(motor2PWM, velocidadInt);

  // 5. MONITOR SERIE (Para ver todo en vivo)
  Serial.print("Freno: ");
  Serial.print(valorFreno);
  Serial.print(" | Embrague: ");
  Serial.print(valorEmbrague);
  Serial.print(" | Marcha: ");
  Serial.print(multiplicadorMarcha);
  Serial.print(" | Vel. Motor: ");
  Serial.println((int)velocidadFinal);

  delay(50);
}
