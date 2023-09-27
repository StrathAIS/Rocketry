clear;

inputValues = inputdlg({'Mass of FFFFg powder (g):'})

grammassBP = str2double(inputValues{1}); %g
lbsmassBP = grammassBP*0.0022; %lbs
bulkheadVolume = 466.55; %cubic inches
gasConstant = 22.16; %ft lb/slug oR
combustionTemperature = 3307; %R

pressureinBulkhead = ((lbsmassBP)*(gasConstant)*(combustionTemperature))/(bulkheadVolume) %psi
papressureinBulkhead = pressureinBulkhead*6894.76
