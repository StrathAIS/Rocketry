import os
import math
import numpy as np
import sys
import pprint as pp
import orhelper
from orhelper import FlightDataType

## MORI: Matlab-OpenRocket Interface
############# Tommy W ##############

# This script defines functions called by Matlab to interface with Openrocket
# through python, orhelper and jpype. It functions in python also but can only
# be called once per python instance. (idk why)

## INPUTS
# target # Name of target OR file (.ork) as a string e.g. 'Strathosphere_mk2_v5.ork' 
# component # Name of component as a string, best to copy directly from OR file e.g. 'Trapezoidal fin set'
# command # String representing java command, found in the OR dictionary e.g. 'setThickness'
# value # Argument of command, if a number should usually be SI, again a string e.g. '0.003'
# outputType # A list of flightDataType strings, as defined in _enums.py

def runORTweak(target,component,command,value):
    with orhelper.OpenRocketInstance() as instance:
        orh = orhelper.Helper(instance)
    
        # Load document, sim, options and rocket...
        doc = orh.load_doc(os.path.join('../','OpenRocket/',target))
        sim = doc.getSimulation(0)
        opts = sim.getOptions()
        rocket = sim.getRocket()
        # Find component
        component = orh.get_component_named(rocket, component)
        # Run Command
        runCommand='component.'+command+'('+value+')'
        eval(runCommand)
        # Save rocket
        orh.save_doc(os.path.join('../','OpenRocket/',target),doc)

def runORSim(target,outputType):
    with orhelper.OpenRocketInstance() as instance:
        orh = orhelper.Helper(instance)
    
        # Load document, sim, options and rocket...
        doc = orh.load_doc(os.path.join('../','OpenRocket/',target))
        sim = doc.getSimulation(0)
        opts = sim.getOptions()
        orh.run_simulation(sim)
        outputCommand='orh.get_timeseries(sim,['
        for i,data in enumerate(outputType):
            outputCommand=outputCommand+(str(data))+','
            
        outputCommand.removesuffix(',')
        outputCommand=outputCommand+'])'
        output=eval(outputCommand)
        return output
if runSim==0:
    runORTweak(target,component,command,value)
else:
    output=runORSim(target,outputType)     
pp.pprint(output)