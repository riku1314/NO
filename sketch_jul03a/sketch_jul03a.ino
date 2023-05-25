#include <fix_fft.h>
#include <Time.h>
#define N_SAMPLES 8192
#define a 0.8


char im[N_SAMPLES];
char data[N_SAMPLES];    // 入力/実数部
int data1[N_SAMPLES];
int mic = 0;      // マイク接続ポート
int prevSec = 0;  // 秒カウント
int max1[7];
int max2[7];
int v;
int f[4][4];
int k;
char rc[N_SAMPLES];

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT);
  for(int i=8;i<11;i++){
    pinMode(i,OUTPUT);
  }
  prevSec = -1;
}

void loop(){
  v=digitalRead(7);
  if(v==HIGH){
    int  i;
    unsigned long mil,mil1;
    double SamplingFrequency;
    int Frequency;
    int tmp;
    for(i=0;i<7;i++){
      max1[i]=0;
      max2[i]=0;
    }
    mil1 = millis();
    for (i = 0; i < N_SAMPLES; i++) {
    // 音声を読み込み
    data[i] = (analogRead(mic) >> 2)-256;
   
    // 虚数部をクリア
    im[i]   = 0;
    }
    mil = millis() - mil1;
    Serial.println(mil);
    rc[0]=(1-a)*data[0];
    for(i=1;i<N_SAMPLES;i++){
      rc[i]=a*rc[i-1]+(1-a)*data[i];
    }
  // 256個の読み込み時間を計測
     Frequency=1/(mil*0.001);
     fix_fft(rc, im, 8, 0);  // full scale 2^8=256, FFT mode
     for (i = 0; i < N_SAMPLES/2; i++) {
      rc[i] = sqrt(rc[i] * rc[i] + im[i] * im[i]);
      data1[i]=rc[i];
      
      if(max1[0]<data1[i]){
        max1[0]=data1[i];
        max2[0]=i*Frequency;
      }
      
      else if(max1[1]<data1[i]){
        max1[1]=data1[i];
        max2[1]=i*Frequency;
      }
      
      else if(max1[2]<data1[i]){
        max1[2]=data1[i];
        max2[2]=i*Frequency;
      }
      
      else if(max1[3]<data1[i]){
        max1[3]=data1[i];
        max2[3]=i*Frequency;
      }
     }
     
     int his;
     Serial.print(k+1);
     Serial.println("回目");
     for(i=0;i<3;i++){
      Serial.print(max2[i]);
      Serial.println("Hz");
      f[k][i]=max2[i];
     }
     
     tone(8,max2[0],1000);
     tone(9,max2[1],1000);
     tone(10,max2[2],1000);
     delay(3000);
     k++;
  }
  
  int c;
  if(Serial.available()>0){
    c=Serial.read();
    if(c=='a'){
      tone(8,f[0][0],1000);
      tone(9,f[0][1],1000);
      tone(10,f[0][2],1000);
      delay(3000);
    }
    else if(c=='b'){
      tone(8,f[1][0],1000);
      tone(9,f[1][1],1000);
      tone(10,f[1][2],1000);
      delay(3000);
    }
    else if(c=='c'){
      tone(8,f[2][0],1000);
      tone(9,f[2][1],1000);
      tone(10,f[2][2],1000);
      delay(3000);
    }
  }
}
