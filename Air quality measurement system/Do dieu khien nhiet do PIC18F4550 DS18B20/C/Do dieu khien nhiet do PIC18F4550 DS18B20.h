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
