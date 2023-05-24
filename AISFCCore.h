/*
  AISFC Core Management header

  This header is the master handler for the FC's capabilitiy.
    **This is Mission Critical**
  Master functions that coordinate between resources and modules are handled here

  Created: 23th May 2023
  Last Update: 23th May 2023
  Created By: Michael Haggart
  For: StarthAIS
  Updated by: Michael Haggart
              #Add New Names Here
*/

#ifndef AISFCCore
#define AISFCCore
//
//
enum flightStatus
{
    preLaunch,
    Boost,
    Coast,
    Apogee,
    Drogue,
    Main,
    Landed
};
//
//
bool descendingCheck(int& sampleCount, float& apogeeAlt, float cAlt);
bool motorCheckFunction(int& accelSampleCount, float absoluteAccel);
flightStatus stateCheckFunc(flightStatus& fS, float timeSinceLaunch, bool apogeeCheck, bool drogueDep, bool mainDep, bool motorCheck, float highestAlt, float curAlt);
//
//
bool descendingCheck(int& sampleCount, float& apogeeAlt, float cAlt) {
    if ((apogeeAlt - cAlt) >= 1)  //if current alt is lower than 1 meter below apogee
    {
        sampleCount = sampleCount + 1;  //counter increments
        apogeeAlt = cAlt;               //apogee = current alt
        if (sampleCount == 15)          //if 15 counts are sucessful return true;
        {
            return true;
        }
        return false;
    }
    if ((apogeeAlt - cAlt) < 1)  //if current alt is greater than 1 meter above apogee
    {
        apogeeAlt = apogeeAlt;  //apogee
        sampleCount = 0;
        return false;
    }
}
//
//
bool motorCheckFunction(int& accelSampleCount, float absoluteAccel)
{
    bool motorCheck = false;
    if (absoluteAccel > 15)
    {
        accelSampleCount = accelSampleCount + 1;
        if (accelSampleCount == 5)
        {
            return motorCheck = true;
        }
        return motorCheck = false;
    }
    else
    {
        accelSampleCount = 0;
        return motorCheck = false;
    }
}
//
//
flightStatus stateCheckFunc(flightStatus& fS, float timeSinceLaunch, bool apogeeCheck, bool drogueDep, bool mainDep, bool motorCheck, float highestAlt, float curAlt)
{
    //The motorCheck is bool derived from the current acceleration of the rocket. If it is above some threshhold, then the motor is currently firing
    //If this is true, then the rocket is in its boost phase
    //If this is false && apogeeCheck is false, the rocket is in its Coast phase. 
    if (apogeeCheck == false && motorCheck == false && drogueDep == false && mainDep == false && highestAlt >= 0 && curAlt <= 50)
    {
        return fS = flightStatus::preLaunch;
    }
    if (apogeeCheck == false && motorCheck == true && drogueDep == false && mainDep == false && highestAlt >= 0 && curAlt >= 10)
    {
        return fS = flightStatus::Boost;
    }
    if (apogeeCheck == false && motorCheck == false && drogueDep == false && mainDep == false && highestAlt >= 0 && curAlt >= 100)
    {
        return fS = flightStatus::Coast;
    }
    if (apogeeCheck == true && motorCheck == false && drogueDep == false && mainDep == false && highestAlt >= 0 && curAlt >= 3000)
    {
        return fS = flightStatus::Apogee;
    }
    if (apogeeCheck == true && motorCheck == false && drogueDep == true && mainDep == false && highestAlt >= curAlt && curAlt >= 2000)
    {
        return fS = flightStatus::Drogue;
    }
    if (apogeeCheck == true && motorCheck == false && drogueDep == true && mainDep == false && highestAlt >= curAlt && (curAlt >= 400 || curAlt <=500))
    {
        return fS = flightStatus::Main;
    }
    if (apogeeCheck == true && motorCheck == false && drogueDep == true && mainDep == true && highestAlt >= curAlt && curAlt <= 50)
    {
        return fS = flightStatus::Landed;
    }
    //if (apogeeCheck == true && motorCheck == true && drogueDep == false && mainDep == false && curAlt >= 350)
    //{
    //    //CREATE EMERGENCY FLIGHT STATUS TO DEPLOY BOTH DROGUE AND MAIN IF CRASHING 
    //}
    else
    {
        return fS = fS;
    }
}
//
//
#endif // !AISFCCore