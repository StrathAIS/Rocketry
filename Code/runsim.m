function v=runsim(target,component,command,value,outputType)
    pyEnv=pyenv("ExecutionMode","OutOfProcess");
    v=pyrunfile("ORpyShell.py","output",target=target,component=component,command=command,value=value,outputType=outputType);
    terminate(pyenv)
end