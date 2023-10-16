%%%Fin Flutter calculator.
%%Input values below;
%Note to put in IMPERIAL
format shortG
%Shear Modulus:
sm = 2020000 ;

%Root Chord:
cr = 9.843 ;

%Tip Chord:
ct = 3.937 ;

%Thickness:
t = 0.118;

%Semi Span:
b = 6.299 ;

%Altitude at max speed 

h = 1804.46 ;

%Safety Factor ?? -within 50%


%%Fin Surface Area 

S = ((cr +ct)/2)* b;

%%Fin Aspect Ratio

Ar = (b^2)/S;

%%The Ratio of tip chord  

ra = ct/cr; 

%%Temperature at altitude 


T = 59 - (0.00356 * h); 

%%Pressure at height 

P = 2116 * ((T+459.7)/518.6)^5.256 ;

Pnew = P/144;

%%Speed of Sound at altitude 


a = sqrt(1.4*1716.59 *(T+460));

%%Main Equation

U1 = (39.3*(Ar)^3)/((t/cr)^3*(Ar +2));

U2 = ((ra+1)/2) * (Pnew/14.7);


Vf = a *(sqrt((sm)/(U1 *U2)));



VF = Vf /3.281;


disp '---------------------------------'
disp 'VF in m/s without SF'
disp(VF)

disp 'Vf in f/s'
disp(Vf)

disp ' U1'
disp(U1)

disp ' U2'
disp(U2)









