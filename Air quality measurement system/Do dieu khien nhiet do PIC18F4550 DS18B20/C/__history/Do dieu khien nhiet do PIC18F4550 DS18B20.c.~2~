#include <18f4550.h>      

#device ADC=16
                                                               
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset          
#FUSES hs,CPUDIV1                   
#fuses MCLR         
                                                                                 
#use delay(clock=4000000)   
#define LCD_ENABLE_PIN  PIN_D2 // khai bao cac chan ket noi PIC voi LCD
#define LCD_RS_PIN      PIN_D0                                  ////
#define LCD_RW_PIN      PIN_D1                                   ////
#define LCD_DATA4       PIN_D4                                    ////
#define LCD_DATA5       PIN_D5                                   ////
#define LCD_DATA6       PIN_D6                                  ////
#define LCD_DATA7       PIN_D7                           
#include <lcd.c>
#include<1wire.c> //truy xuat vao file 1wire
void tao_tre_quet_nut();
void ht_chinh();
void tang_gt();
void giam_gt();
void hien_thi_dat();
void doc_bo_nho();
void luu_bo_nho();
void so_sanh();
#include<ds1820.c> //truy xuat vao file ds1820.c 
               
      float ds1820_read() 
{ 
 int8 temp1, temp2; 
 signed int16 temp3; 
 float result; //khai bao bien tra ve ket qua sau doan chuong trinh

 onewire_reset(); //lenh reset  
 ghi_1wire(0xCC); //bo qua truy xuat rom
 ghi_1wire(0x44); //Lenh chuyen doi nhiet do 
 delay_ms(100); //Tao tre chuyen doi nhiet do

 onewire_reset();  //lenh reset  
 ghi_1wire(0xCC); //lenh bo qua truy xuat rom 
 ghi_1wire(0xBE); //Lenh doc du lieu tu ds18b20 
 temp1 = doc_1wire(); //doc byte 0 8 bit
 temp2 = doc_1wire(); //doc byte 1 8 bit
 temp3 = make16(temp2, temp1); //noi hai byte thanh tu 16 bit 
 
//result = (float) temp3 / 2.0;   //Calculation for DS18S20 with 0.5 deg C resolution 
 result = (float) temp3 / 16.0;  //Chuyen doi nhiet do ds18b20 thanh dang so thuc 16 bit
 return(result); 
} 
               
void so_sanh(); // khai bao ham so sanh 
void quet_nut();
   
#define led_xanh pin_c0 
#define led_do pin_c1 
#define rl1 pin_a0 
#define rl2 pin_a1  
int8 chinh=0;
#define tang pin_b0 // tang // dinh nghia cac chan nut nhan
#define giam pin_b1 // giam
#define mode pin_b2 // mode
int1 np=0; // khai bao bit nho co phim nhan, np=0 co phim nhan, np=1 tat ca cac phim deu nha 
float nhiet_do; //khai bao bien nhiet do so thuc 32 bit 
float nhiet_dat_l=101.1; 
float nhiet_dat_h=121.2; 
//int8 a=034;
int1 bct; // bit tao chop tat
int16 ct; // dem thoi gian chop tat
//read_eeprom
void main()               
{                             
   SETUP_ADC(ADC_OFF); // tat che do adc cua pic
   lcd_init(); // khoi tao lcd           
   //output_float(DHT11_pin); // tha noi chan cam bien                   
   //delay_ms(1000); // tao tre cho cam bien san sang                                                                                       
   //lcd_gotoxy(1, 1); // dua con tro lcd toi cot 1 dong 1       
   //lcd_putc("DO AM DO : 99.0%"); // xuat hie thi len lcd                      
   lcd_gotoxy(1, 2); // dua co tro lcd den cot 1 dong 2
   lcd_putc("L:120.0 -H:100.1 "); // xuat hien thi len lcd  
   //lcd_gotoxy(12, 2);  
   //printf(lcd_putc, "%02lu.0",do_am_dat); 
   
   doc_bo_nho();

   while(true)  // vong lap vo tan                                                          
   { 
      nhiet_do = ds1820_read();
      nhiet_do/=10;
      //get_data();  // doc gia tri tu cam bien           
//!      lcd_gotoxy(12, 1);
//!      printf(lcd_putc, "%02u.%01u", gia_tri[0], gia_tri[1]); 
      
      lcd_gotoxy(1,1); //dua con tro tren lcd ve dong 1 cot 1 
      printf(lcd_putc,"Nhiet: %03.1f ",nhiet_do); //xuat len lcd nhiet do do duoc 
      lcd_putc(223); //xuat len lcd ki tu do 
      lcd_putc("C    "); //xuat len ldc ki tu C 
      if(chinh==0)
      {
         hien_thi_dat();
      }
      ht_chinh();
      so_sanh();  
      //tao_tre_quet_nut(); // tao thoi gian tre cho cam biên san sang       
   }                                    
}
//======================================== 
void doc_bo_nho()
{
   int8 h;
   int8 l;
   int8 kt;
   kt=read_eeprom(0);
   if(kt==0x55)
   {
      l=read_eeprom(1);
      h=read_eeprom(2);
      nhiet_dat_l=make16(h,l);
      nhiet_dat_l/=10;
      l=read_eeprom(3);
      h=read_eeprom(4);
      nhiet_dat_h=make16(h,l);
      nhiet_dat_h/=10;
   }
   else
   {
      write_eeprom(0,0x55);
      nhiet_dat_l=20;
      nhiet_dat_h=35;
      luu_bo_nho();
   }
}
//======================================== 
void luu_bo_nho()
{
   int8 h;
   int8 l;
   int16 a=nhiet_dat_l*10;
   l=make8(a,0);
   h=make8(a,1);
   write_eeprom(1,l);
   write_eeprom(2,h);
   
   a=nhiet_dat_h*10;
   l=make8(a,0);
   h=make8(a,1);
   write_eeprom(3,l);
   write_eeprom(4,h);
}
//======================================== 
void hien_thi_dat()
{
   int16 a;
   a=nhiet_dat_l;
   lcd_gotoxy(1,2); //dua con tro tren lcd ve dong 1 cot 1 
   //printf(lcd_putc,"L:%03.1f -H:%03.1f", nhiet_dat_l,nhiet_dat_h); //xuat len lcd nhiet do do duoc
   printf(lcd_putc,"L:%03lu",a);
   printf(lcd_putc,".");
   a=nhiet_dat_l*10;
   a%=10;
   printf(lcd_putc,"%lu -H:",a);
   a=nhiet_dat_h;
   printf(lcd_putc,"%03lu",a);
   printf(lcd_putc,".");
   a=nhiet_dat_h*10;
   a%=10;
   printf(lcd_putc,"%lu",a);
}
//======================================== 
void tao_tre_quet_nut()
{
   int8 i;
   for(i=0;i<75;i++)
   {                                 
      quet_nut();
      delay_ms(10); // tao thoi gian tre ms 
      if(chinh!=0)
      {
         ct++;
         if(ct>15)
         {
            bct=!bct;
         }
      }
   }
} 
//========================================
void quet_nut() // chuong trinh con quet nut nhan
{
   if (np)
   {                             
      if (!input(tang))
      {
         np=0; // bao co nut duoc nhan
         bct=1;
         ct=0;
         tang_gt();                                                  
         hien_thi_dat();
      }
//----------------------------------------
      if (!input(giam))
      {                                                         
         np=0;
         bct=1;
         ct=0;
         giam_gt();  
         hien_thi_dat();            
      }
      if (!input(mode))
      {                                                         
         np=0;
         bct=0;
         ct=0;
         chinh++;
         if(chinh>8)
         {
            chinh=0;
            luu_bo_nho();
         }
         hien_thi_dat();
         ht_chinh();
      }
   }
   else
   {
      if((input(tang)==1)&&(input(giam)==1)&&(input(mode)==1))
      {
         np=1; // bao khong co nut nhan 
      }
   }
}
//======================================== 
void tang_gt()
{
   int16 so_dat;
   if(chinh<5)
   {
      so_dat=nhiet_dat_l*10;
      if(chinh==4)
      {
         so_dat++;
         if(so_dat%10==0)
         {
            so_dat-=10;
         }
      }
      if(chinh==3)
      {
         so_dat+=10;
         if(so_dat/10%10==0)
         {
            so_dat-=100;
         }
      }
      if(chinh==2)
      {
         so_dat+=100;
         if(so_dat/100%10==0)
         {
            so_dat-=1000;
         }
      }
      if(chinh==1)
      {
         so_dat+=1000;
         if(so_dat/1000%2==0)
         {
            so_dat-=2000;
         }
      }
      nhiet_dat_l=so_dat;
      nhiet_dat_l/=10;
   }
   else
   {
      so_dat=nhiet_dat_h*10;
      if(chinh==8)
      {
         so_dat++;
         if(so_dat%10==0)
         {
            so_dat-=10;
         }
      }
      if(chinh==7)
      {
         so_dat+=10;
         if(so_dat/10%10==0)
         {
            so_dat-=100;
         }
      }
      if(chinh==6)
      {
         so_dat+=100;
         if(so_dat/100%10==0)
         {
            so_dat-=1000;
         }
      }
      if(chinh==5)
      {
         so_dat+=1000;
         if(so_dat/1000%2==0)
         {
            so_dat-=2000;
         }
      }
      nhiet_dat_h=so_dat;
      nhiet_dat_h/=10;
   }
}
//---------------------------------------- 
void giam_gt()
{
   int16 so_dat;
   if(chinh<5)
   {
      so_dat=nhiet_dat_l*10;
      if(chinh==4)
      {
         if(so_dat/1%10==0)
         so_dat+=9;
         else
         {
            so_dat-=1;
            if(so_dat/1%10==9)
            {
               so_dat+=10;
            }
         }
      }
      if(chinh==3)
      {
         if(so_dat/10%10==0)
         so_dat+=90;
         else
         {
            so_dat-=10;
            if(so_dat/10%10==9)
            {
               so_dat+=100;
            }
         }
      }
      if(chinh==2)
      {
         if(so_dat/100%10==0)
         so_dat+=900;
         else
         {
            so_dat-=100;
            if(so_dat/100%10==9)
            {
               so_dat+=1000;
            }
         }
      }
      if(chinh==1)
      {
         if(so_dat/1000%10==0)
         so_dat+=1000;
         else
         {
            so_dat-=1000;
//!            if(so_dat/1000%10==9)
//!            {
//!               so_dat+=1000;
//!            }
         }
      }
      nhiet_dat_l=so_dat;
      nhiet_dat_l/=10;
   }
   else
   {
      so_dat=nhiet_dat_h*10;
      if(chinh==8)
      {
         if(so_dat/1%10==0)
         so_dat+=9;
         else
         {
            so_dat-=1;
            if(so_dat/1%10==9)
            {
               so_dat+=10;
            }
         }
      }
      if(chinh==7)
      {
         if(so_dat/10%10==0)
         so_dat+=90;
         else
         {
            so_dat-=10;
            if(so_dat/10%10==9)
            {
               so_dat+=100;
            }
         }
      }
      if(chinh==6)
      {
         if(so_dat/100%10==0)
         so_dat+=900;
         else
         {
            so_dat-=100;
            if(so_dat/100%10==9)
            {
               so_dat+=1000;
            }
         }
      }
      if(chinh==5)
      {
         if(so_dat/1000%10==0)
         so_dat+=1000;
         else
         {
            so_dat-=1000;
//!            if(so_dat/1000%10==9)
//!            {
//!               so_dat+=11000;
//!            }
         }
      }
      nhiet_dat_h=so_dat;
      nhiet_dat_h/=10;
   }
}
//======================================== 
void so_sanh()                                                      
{
   if(nhiet_do<nhiet_dat_l) 
   {       
      output_high(led_xanh); // dua chan len muc cao     
      output_low(led_do); // dua chan xuong muc thap
      output_high(rl1); // dua chan len muc cao     
      output_low(rl2); // dua chan xuong muc thap
   }     
   else       
   if(nhiet_do>nhiet_dat_h) 
   {        
      output_high(led_do); // dua chan len muc cao      
      output_low(led_xanh); // dua chan xuong muc thap 
      output_high(rl2); // dua chan len muc cao      
      output_low(rl1); // dua chan xuong muc thap 
   }
   else
   {
      output_low(led_do); // dua chan xuong muc thap 
      output_low(led_xanh); // dua chan xuong muc thap
      output_low(rl1); // dua chan xuong muc thap 
      output_low(rl2); // dua chan xuong muc thap 
   }                                            
}                                                                           
//========================================
void ht_chinh()
{
   if(chinh!=0)
   {
      if(bct==1)
      {
         hien_thi_dat();
      }
      if((!bct&&(chinh==1)))
      {
         lcd_gotoxy(3,2); //dua con tro tren lcd ve dong 1 cot 1 
         printf(lcd_putc," "); //xuat len lcd nhiet do do duoc
      }
      if((!bct&&(chinh==2)))
      {
         lcd_gotoxy(4,2); //dua con tro tren lcd ve dong 1 cot 1 
         printf(lcd_putc," "); //xuat len lcd nhiet do do duoc
      }
      if((!bct&&(chinh==3)))
      {
         lcd_gotoxy(5,2); //dua con tro tren lcd ve dong 1 cot 1 
         printf(lcd_putc," "); //xuat len lcd nhiet do do duoc
      }
      if((!bct&&(chinh==4)))
      {
         lcd_gotoxy(7,2); //dua con tro tren lcd ve dong 1 cot 1 
         printf(lcd_putc," "); //xuat len lcd nhiet do do duoc
      }
      if((!bct&&(chinh==5)))
      {
         lcd_gotoxy(12,2); //dua con tro tren lcd ve dong 1 cot 1 
         printf(lcd_putc," "); //xuat len lcd nhiet do do duoc
      }
      if((!bct&&(chinh==6)))
      {
         lcd_gotoxy(13,2); //dua con tro tren lcd ve dong 1 cot 1 
         printf(lcd_putc," "); //xuat len lcd nhiet do do duoc
      }
      if((!bct&&(chinh==7)))
      {
         lcd_gotoxy(14,2); //dua con tro tren lcd ve dong 1 cot 1 
         printf(lcd_putc," "); //xuat len lcd nhiet do do duoc
      }
      if((!bct&&(chinh==8)))
      {
         lcd_gotoxy(16,2); //dua con tro tren lcd ve dong 1 cot 1 
         printf(lcd_putc," "); //xuat len lcd nhiet do do duoc
      }
   }
}
