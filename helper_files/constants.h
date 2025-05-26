#include <bits/stdc++.h>

std::string engineInts[] = {
    "MINIMUM",
    "LIMITER",
    "LIMITER_HZ",

    "RPM",
    "TORQUE",
    "NON_LINEARITY",

    "COCKPIT_ADJUSTABLE",
    "REFERENCE_RPM",

    "RPM_THRESHOLD"
};

std::string engineDoubles[] = {
    "ALTITUDE_SENSITIVITY",
    "INERTIA",

    "LAG_DN",
    "LAG_UP",
    "MAX_BOOST",
    "WASTEGATE",
    "DISPLAY_MAX_BOOST",
    "GAMMA",

    "TURBO_BOOST_THRESHOLD",
    "TURBO_DAMAGE_K",
    "RPM_DAMAGE_K"
};

std::string engineSettingNames[] = {
    "Torque curve File",
    "Altitude Sensitivity",
    "Inertia",
    "Idle RPM",
    "Redline RPM",
    "Limiter HZ",
    "Coast Reference RPM",
    "Coast Torque",
    "Coast Nonlinearity",
    "Turbo Lag Spooling Down",
    "Turbo Lag Spooling Up",
    "Max Boost",
    "Wastegate Pressure",
    "Max Display Boost (for use in game UI)",
    "RPM at which max boost is reached",
    "Boost Gamma",
    "Boost adjustable from cockpit?",
    "Turbo damage threshold pressure",
    "Turbo damage factor",
    "Engine damage threshold RPM",
    "Overrev damage factor"
};