

int LED_1 = 25;

void setup() {


  pinMode(LED_1, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  delay(1000);
  digitalWrite(LED_1,HIGH);
  delay(1000);
  digitalWrite(LED_1,LOW);


}
