function output=ORSim(target,outputType)
%% MORI: Matlab - OpenRocket Interface
%%%%%%%%%%%%%% Tommy W %%%%%%%%%%%%%%%
% ORSim.m allows the running of a simulation in OpenRocket through matlab before outputting flight data
%% Inputs
%  target - Name of target OR file (.ork) as a string e.g. 'Strathosphere_mk2_v5.ork' 
%           note that this file must be saved in a directory called "OpenRocket", one directory up from the working directory
%           this is to ensure good compliance with the StrathAIS rocketry github
%  outputType - A cell array of flightDataType strings, as defined in the MORI Dictionary
    pyEnv=pyenv("ExecutionMode","OutOfProcess");
%     try % Ensure process is terminated even if python throws an error
        output=pyrunfile("ORpyShell.py","output",target=target,outputType=outputType,runSim=1)
%     end
    terminate(pyenv)
end