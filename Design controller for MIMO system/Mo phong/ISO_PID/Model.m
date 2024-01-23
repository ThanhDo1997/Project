f=[0,0];
f(1)=0.8e-5;
f(2)=1.6e-5;

f3_max= 4.7e-5;
k= f3_max/sqrt(0.3);
syms x1 x2;

Th=70;
Tc=30;
h_T=[0 0]; 
h_T(1)=((f(1)+f(2))/k)^2;
h_T(2)= (f(1)*Tc+ f(2)*Th)/(f(1)+f(2));

% Parameter
Ar = 3.24e-2;
Th = 70;
Tc = 31;
g = 9.8;
Av = 7.8e-5;  %cross area 
v = 0.5;    %open-rate
% k = Av*sqrt(2*g) * v;


% Mo hinh sau khi tuyen tinh hoa
A = [-k/(2*Ar*sqrt(h_T(1))) 0;
     0 -k/(Ar*sqrt(h_T(1)))];
B = [1/Ar 1/Ar;
    (Tc-h_T(2))/(Ar*h_T(1)) (Th-h_T(2))/(Ar*h_T(1))];
C = [1 0; 0 1];
D = zeros(2,2);

% Transfer function
[num1,den1]=ss2tf(A,B,C,D,1);
[num2,den2]=ss2tf(A,B,C,D,2);
G11=tf(num1(1,:),den1);
G21=tf(num1(2,:),den1);
G12=tf(num2(1,:),den2);
G22=tf(num2(2,:),den2);
G=[G11,G12;G21,G22];
G=minreal(G);

% ISO
s=tf('s');
h0=[1/(4*s+1) 0;0 1/(2*s+1)];
hc=inv(G)*h0*inv(eye(2)-h0);
hc=minreal(hc);