void setup() {
  for(int i = 38; i <= 52;i++){
    pinMode(i,OUTPUT);
  }
}

bool val = false;
void loop() {
  val = !val;
  for(int i = 38; i <= 52;i++){
    digitalWrite(i,val);
  }
  delay(1000);
}
