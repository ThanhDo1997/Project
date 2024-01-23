#include <SPI.h>
#include "AD770x.h"
#include "MCP492X.h"
#include <avr/interrupt.h>
#include <math.h>


#define CS1 10
#define CS2 9
#define CS3 8
#define CS4 7
#define pwm2 6 
#define pwm1 5
#define CS5 4  
#define pi 3.14
 

//ADC
AD770X adc1(CS1);
AD770X adc2(CS2);
AD770X adc3(CS3);
AD770X adc4(CS4);
//DAC
MCP492X dac(CS5);
unsigned long adc_value1=10,adc_value2=11,adc_value3=12,adc_value4=13;
float T_hot=0,T_cold=0,T=0,h=0;

long dac_value1=0, dac_value2=0;

////////////model params
float Area= 3.42E-2;
float g=9.81;
//pipe cross erea
float a0= pi*(0.021/2)*(0.021/2);
//valve open
float v=0.5;
float alpha=sqrt(2*g)*v*a0; //f_valve= alpha*sqrt(h)

//////////// mesurement params
float Vcc=4.993;
int K=8;
float R_ref= 3300;
float R_ref_h= 220.0;
long K1= K*65536;
float V_ref_adc=2.485;
float R=100.0;  

////////////Pt100 params
float A=3.9083E-3;
float R0=101.34; // RTD at 33 degrees is 114,4

///////////SMC params
float T_set=50;
float h_set=0.20;
float upper_limit_cold= 2.28E-5;
float upper_limit_hot= 4.28E-5;
float k_1=0.21;
float k_2=0.037;
float f3_max = 4.7E-5; // 0.3 is h max
float f1=0.0,f2=0.0; // output of controller, f1--cold, f2--hot
////////////


  int counter=3037;
void timmer_init()
    {
        cli();                                  // disable global interrupt
        
        /* Reset Timer/Counter1 */
        TCCR1A = 0;
        TCCR1B = 0;
        TIMSK1 = 0;
        
        // timer 1 freq= 16Mhz
        TCCR1B |= (1 << CS12) | (0 << CS10);    // prescale = 256
        TCNT1 = 34286;                            // period =0.5s
        counter=34286;
        TIMSK1 = (1 << TOIE1);                  // Overflow interrupt enable 
        sei();         
    }

void control_intertupt(bool active)
{
  if(active==true)
  {
    TIMSK1 = TIMSK1 | (1 << TOIE1);
    sei();
  }
  else
  {
    TIMSK1 = TIMSK1 & (~(1 << TOIE1));
    sei();
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(12,INPUT);//MISO
  pinMode(13,OUTPUT);//SCK
  pinMode(11,OUTPUT);//MOSI
  Serial.println("stuck0");
  pinMode(CS1,OUTPUT);
  pinMode(CS2,OUTPUT);
  pinMode(CS3,OUTPUT);
  pinMode(CS4,OUTPUT);
  pinMode(CS5,OUTPUT);
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  
  Serial.begin(115200);
  delay(10);
  SPI.begin();
  delay(10);
  
  adc1.reset();
  delay(10);
  adc1.init(AD770X::CHN_AIN2,AD770X::GAIN_8);
  delay(10);
  Serial.println("stuck1");
  delay(10);
  
  adc2.reset();
  delay(10);
  adc2.init(AD770X::CHN_AIN2,AD770X::GAIN_8);
  delay(10);
  Serial.println("stuck2");
  delay(10);
  
  adc3.reset();
  delay(10);
  adc3.init(AD770X::CHN_AIN2,AD770X::GAIN_8);
  delay(10);
  Serial.println("stuck3");
  delay(10);
  
  adc4.reset();
  delay(10);
  adc4.init(AD770X::CHN_AIN2,AD770X::GAIN_1);
  delay(10);
  Serial.println("stuck4");
  delay(10);

  dac.begin();
  delay(10);
  
  timmer_init();
}


void read_adc_n(int n){
  float tmp1=0,tmp2=0,tmp3=0,tmp4=0;
  for(int i=0;i<n;i++)
  {
    
    tmp1+= adc1.readADResultRaw(AD770X::CHN_AIN2);
    delay(2);
    
  
    tmp2+= adc2.readADResultRaw(AD770X::CHN_AIN2);
    delay(2);

    tmp3+= adc3.readADResultRaw(AD770X::CHN_AIN2);
    delay(2);
    tmp4+= adc4.readADResultRaw(AD770X::CHN_AIN2);
    delay(2);
  }
  control_intertupt(false);
  adc_value1= tmp1/n;
  adc_value2= tmp2/n;
  adc_value3= tmp3/n;
  adc_value4= tmp4/n;
  control_intertupt(true);
}

void get_feedback()
{
  read_adc_n(20);
  float RTD1=  R / (-1+1/( 0.5+ 2*adc_value1*V_ref_adc/(K1*Vcc) ) );
  float RTD2=  R / (-1+1/( 0.5+ 2*adc_value2*V_ref_adc/(K1*Vcc) ) );
  float RTD3= R / (-1+1/( 0.5+ 2*adc_value3*V_ref_adc/(K1*Vcc) ) );//wheaston bridge
  float i_H= adc_value4*2.0*V_ref_adc
            /(65536*R_ref_h);

  T_hot= (RTD1/R0-1)/A;
  Serial.print("Nhiet do T_hot");
  Serial.println(T_hot);
  T_cold= (RTD2/R0-1)/A-8.8;
  Serial.print("Nhiet do T_cold");
  Serial.println(T_cold);
  T= (RTD3/R0-1)/A - 4.3;
  Serial.print("Nhiet do T");
  Serial.println(T);
  h=12987*i_H-48.38;// experiment equation in excel
  h=h/100;
  Serial.print("do cao h");
  Serial.println(h);
}


/////////////////////////////////////////////////////////////////
//////ISO
float k12=0.00381, kI_12=1.21e-6;
float k11=0.00277, kI_11=2.62e-6;
float k22=0.103e-5, kI_22=3.85e-8;
float k21=-1.03e-5, kI_21=-3.85e-8;
float dt=0.5;
float e_h_sum=0;
float e_T_sum=0;
float h_balance= 0.12;
float T_balance= 56;
//////PID
float kp1=2;
float ki1=0.125;
float kp2=1.5;
float e_sum1=0;
void tach_kenh()
{   
  float role_h= h- h_balance;
  float role_T= T- T_balance;
  float e_delta_h = h_set-h;
  float e_delta_T = T_set-T;
  
  if(role_h < 0){
    f1=upper_limit_cold;
    float f2 = kp2*e_delta_T;
    if(f2<0)f2=0;
    else if(f2>upper_limit_hot)f2=upper_limit_hot;
    }//Neu chua qua muc balance thi thuc hien khau role, chua thuc hien PID
  else{
        float f1= kp1*e_delta_h + ki1*e_sum1;//
        float f2 = kp2*e_delta_T;
        if(f1<0){
            e_sum1 = e_sum1 + dt*(e_delta_h - (f1 - 0)/3);
            f1 = 0;
          }
        else if(f1>upper_limit_cold){
             e_sum1 = e_sum1 + dt*(e_delta_h - (f1 - upper_limit_cold)/3);
             f1=upper_limit_cold;
          }
        else e_sum1 = e_sum1 + dt*e_delta_h;
    

  }
  

  
  float alpha_valve_cold= f1/upper_limit_cold;
  float alpha_valve_hot= f2/upper_limit_hot;

  float i_out_cold = -alpha_valve_cold*16.0+ 20.0;
  float i_out_hot = -alpha_valve_hot*16.0+ 20.0;
  
  
  dac_value1= i_out_cold/20.0*4095.0;
  dac_value2= i_out_hot/20.0*4095.0;
  dac.analogWrite(pwm1,dac_value1);
  dac.analogWrite(pwm2,dac_value2);
  Serial.println(dac_value1);
  Serial.println(dac_value2);
  //delay(10);
  
}

void loop() {

}

ISR (TIMER1_OVF_vect) 
{
  TCNT1 = counter;
  get_feedback();
  tach_kenh();
}
