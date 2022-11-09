clear;
%All values are SI unit unless stated

%Main

%input
inputValues = inputdlg({"Diameter (m):",'Height (m):','Seperation length (m):','Mass nose (kg):' ...
    ,'Mass body (kg):', 'Shear pin force (N):','Other resistance (N):', ...
    'Air pressure difference [out - in](Pa):','Ejection velocity (m/s):'});

diameter = str2double(inputValues{1});
height = str2double(inputValues{2});
sepLength = str2double(inputValues{3});
massNose = str2double(inputValues{4});
massBody = str2double(inputValues{5});
shearForce = str2double(inputValues{6});
%interferance fit and air resistance combined value,   
% approximate 480N of air resistance assumed 100N of friction fit
otherResistance = str2double(inputValues{7});  
% (+) pressure - pressure oustide is greater than inside,(not possible)
% (-) pressure - pressue inside it greater than outside 
pressureDiff = str2double(inputValues{8});
velocity = str2double(inputValues{9});

%Calculations:

area = areaCalc(diameter); 
initialVolume = volumeCalc(height,area);

%force due to unequalised pressure between bulkhead and atmosphere
%equal pressure : no force
pressureForce = pressureDiff * area;

%ejection force function outputs force in addition to avg acceleration and time

[initialEjecForce,avgAccel,ejecTime] = ejecForceCalc(shearForce, ...
    pressureForce,otherResistance,velocity,massNose,sepLength);

initialPressure = pressureCalc(area,initialEjecForce);
%for this specific case
gasConstant = initialPressure * initialVolume;

%reccuring calc for graph output
%splits time into 1000 equal incriments for calcualtions
x =linspace(0,ejecTime,1000);

%init array
velocitys(1000) = 0;

%calcualting initial value for pressure
% is far too low but is corrected later on
pressures = gasConstant./((volumeCalc(((0.5 *avgAccel*x.^2) + height),area)));

%increases pressure of bulkhead to reach desired final velocity
while velocitys(1000)< velocity
for i = 1:length(pressures)
    pressures(i)= pressures(i) + 100;
end    

%reccurign calc for graph output
forces = pressures .*area;
unbalancedForces = (1:1000);

%calculating unbalanced force for acceleration of nose cone
oppForce = pressureForce + otherResistance;
for i = 1 : length(forces)
    unbalancedForces(i) = forces(i) - oppForce;
end

accelerations = unbalancedForces.*massNose;
velocitys = accelerations.*x;
displacements = (0.5).*accelerations.*x.^2;
end

%black powder req (in imperial units)
impPressure = pressures(1) * 0.000145038; %PSI
impVolume = initialVolume * (39.3700787402^3); %in^3

%formula  and values from insanerocketry.com/blackpowder.html
gramBP = (454/1)*((impPressure*impVolume)/(266*3307));

%setting graphs
pressureAndForce = figure;
accelTime = figure;
disp = figure;

%Pressure & Force vs Time
figure(pressureAndForce)
yyaxis left
title('Pressure and Force vs Time')
xlabel('Time(s)')
ylabel('Pressure (Pa)')
yyaxis right
ylabel('Force(N)')
hold on
yyaxis left
plot(x,pressures)
yyaxis right
plot(x,forces)
hold off
grid on

%Acceleration & Velocity vs Time

figure(accelTime)
yyaxis left
title('Acceleration vs Time')
xlabel('Time(s)')
ylabel('Acceleration(m/s^2)')
yyaxis right
ylabel('Velocity (m/s)')
hold on
yyaxis left
plot(x,accelerations)
yyaxis right
plot(x,velocitys)
hold off
grid on


%Displacement vs Time
figure(disp)
title('displacement vs Time')
xlabel('Time(s)')
ylabel('displacement (m)')
hold on
plot(x,displacements)
hold off
grid on

%Output
fprintf('Ejection force initially on bulkhead: %.3f N\n\n',forces(1))
fprintf('Ejection pressure initially on bulkhead: %.3f Pa\n\n',pressures(1))
fprintf('Blackpowder required: %.3f g\n\n',gramBP)

%Functions

function area = areaCalc(diameter)    
area = (diameter^2*3.141592)/4;
end


function volume = volumeCalc(height,area)
volume = area * height;
end

function [ejecForce,avgAccel,ejecTime] = ejecForceCalc(shear,pressureForce,other,velocity,mass,length)
%time between charge and bottom edge of shoulder leaving main body(assume BP explosion is instant) 
ejecTime = (2*length)/(velocity); 
avgAccel = (2*length)/(ejecTime^2);
accelForce = mass*avgAccel;
%total force required for ejection at desired exit velocity 
ejecForce = shear + pressureForce + other + accelForce; 
end

function pressureBulkhead = pressureCalc(area,force)
pressureBulkhead = force/area;
end