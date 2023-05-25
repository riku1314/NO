const int MIC=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int v;
  v=analogRead(MIC);
  Serial.println(v);
}
