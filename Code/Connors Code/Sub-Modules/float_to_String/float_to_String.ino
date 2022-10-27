


void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
    float f = 1.123456789;
    char c[50]; //size of the number
    Serial.println("COnverting..");
    sprintf(c, "%g", f);
    Serial.print("String:");
    Serial.print(c);
    
    //printf(c);
    //printf("\n");
}

void loop() {
  // put your main code here, to run repeatedly:

}
