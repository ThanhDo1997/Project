int16 ds1820_read()   
{
   //int16 i;
   int8 temp1, temp2;  
   int16 temp3;                               
   float result; //khai bao bien tra ve ket qua sau doan chuong trinh
                                                                
   onewire_reset(); //lenh reset                                                                                                                          
   ghi_1wire(0xCC); //bo qua truy xuat rom                 
   ghi_1wire(0x44); //Lenh chuyen doi nhiet do 
   tao_tre_quet_nut();
   //delay_ms(750);
   onewire_reset();  //lenh reset  
   ghi_1wire(0xCC); //lenh bo qua truy xuat rom 
   ghi_1wire(0xBE); //Lenh doc du lieu tu ds18b20                                      
   temp1 = doc_1wire(); //doc byte 0 8 bit
   temp2 = doc_1wire(); //doc byte 1 8 bit
   temp3 = make16(temp2, temp1); //noi hai byte thanh tu 16 bit         
   result =  temp3 / 16.0;  //Chuyen doi nhiet do ds18b20 thanh dang so thuc 16 bit
   result*=10; // 
   return(result);  
} 
