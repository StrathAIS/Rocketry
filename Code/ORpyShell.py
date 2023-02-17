# -*- coding: utf-8 -*-
"""
Created on Thu Feb 16 17:23:00 2023

@author: tommy
"""

# -*- coding: utf-8 -*-
"""
Created on Tue Feb 14 10:48:17 2023

@author: tommy
"""

import os
import math
import numpy as np
import sys

import orhelper
from orhelper import FlightDataType

# target # Name of target OR file (.ork) as a string e.g. 'Strathosphere_mk2_v5.ork' 
# component # Name of component as a string, best to copy directly from OR file e.g. 'Trapezoidal fin set'
# outputType # List of desired outputs, from _enmums.py, supplied at the end of this script

def runORTweak(target,component,command,value,outputType):
    with orhelper.OpenRocketInstance() as instance:
        orh = orhelper.Helper(instance)
    
        # Load document, sim, options and rocket...
        doc = orh.load_doc(os.path.join('../','OpenRocket/',target))
        sim = doc.getSimulation(0)
        opts = sim.getOptions()
        rocket = sim.getRocket()
        # Pull fin data out...
        component = orh.get_component_named(rocket, component)
        # Run Command
        runCommand='component.'+command+'('+value+')'
        eval(runCommand)
        # Run sim
        orh.run_simulation(sim)
        # Pull maximal velocity
        output=orh.get_timeseries(sim, outputType)
        return output
vmax=runORTweak(target,component,command,value,outputType)