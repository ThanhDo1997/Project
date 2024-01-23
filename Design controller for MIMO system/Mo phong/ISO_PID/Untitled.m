s = tf('s');
G =1/(s+5*10^-4);
step(G)
syms s t
G =1/(s+5*10^-4);
H1 = (1/s)*G;
c = ilaplace(H1)
A = diff(c,t,2);
x = solve(A)