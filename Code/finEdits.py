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
# thickness # FIn Thickness in cm

def runORSim(target,thickness):
    with orhelper.OpenRocketInstance() as instance:
        orh = orhelper.Helper(instance)
    
        # Load document, sim, options and rocket...
        doc = orh.load_doc(os.path.join('../','OpenRocket/',target))
        sim = doc.getSimulation(0)
        opts = sim.getOptions()
        rocket = sim.getRocket()
        # Pull fin data out...
        fins = orh.get_component_named(rocket, 'Trapezoidal fin set')
        # Change fin thickness
        fins.setThickness(thickness)
        # Run sim
        orh.run_simulation(sim)
        # Pull maximal velocity
        v=orh.get_timeseries(sim, [FlightDataType.TYPE_VELOCITY_TOTAL,FlightDataType.TYPE_VELOCITY_Z])
        vmax=np.amax(v[FlightDataType.TYPE_VELOCITY_TOTAL])
        return vmax
vmax=runORSim(target,thickness)