function v=finThickness(target,thickness) % Tommy W
    %% Wrapper function for python script finEdits.py
    %% Inputs - passes the following variables...
    % target - string representing target OpenRocket .ork file in OpenRocket folder e.g. 'Strathosphere_mk2_v5.ork'
    % thickness - fin thickness in metres
    %% Outputs the following variable...
    % v - the maximal total velocity reached by the rocket in m/s

    pyEnv=pyenv("ExecutionMode","OutOfProcess"); % Separate python and matlab processes to prevent crashing
    v=pyrunfile("finEdits.py","vmax",target='Strathosphere_mk2_v5.ork',thickness=thickness) % Run script
    terminate(pyenv) % Kill python to allow restarting of JVM
end