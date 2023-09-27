function OREdit(target,component,command,value)
%% MORI: Matlab - OpenRocket Interface
%%%%%%%%%%%%%% Tommy W %%%%%%%%%%%%%%%
% OREdit.m allows the tweaking of a variable in OpenRocket through matlab before re-saving the .ork file
%% Inputs
%  target - Name of target OR file (.ork) as a string e.g. 'Strathosphere_mk2_v5.ork' 
%           note that this file must be saved in a directory called "OpenRocket", one directory up from the working directory
%           this is to ensure good compliance with the StrathAIS rocketry github
%  component - Name of component as a string, best to copy directly from OR file e.g. 'Trapezoidal fin set'
%  command - String representing java command, found in the MORI dictionary e.g. 'setThickness'
%  value - Argument of command, if a number should usually be SI, again a string e.g. '0.003'
    pyEnv=pyenv("ExecutionMode","OutOfProcess");
    try % Ensure process is terminated even if python throws an error
        pyrunfile("ORpyShell.py",target=target,component=component,command=command,value=value,runSim=0);
    end
    terminate(pyenv)
end