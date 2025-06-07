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

std::string engineSets[] = {
    "ALTITUDE_SENSITIVITY",
    "INERTIA",

    "MINIMUM",
    "LIMITER",
    "LIMITER_HZ",

    "RPM",
    "TORQUE",
    "NON_LINEARITY",

    "LAG_DN",
    "LAG_UP",
    "MAX_BOOST",
    "WASTEGATE",
    "DISPLAY_MAX_BOOST",
    "REFERENCE_RPM",
    "GAMMA",
    "COCKPIT_ADJUSTABLE",

    "TURBO_BOOST_THRESHOLD",
    "TURBO_DAMAGE_K",
    "RPM_THRESHOLD",
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

std::map <std::string,std::string> engineExplanations {
    {"Torque curve File","This will show the torque curve and allow editing it"},
    {"Altitude Sensitivity","Higher value = More sensitive to altitude (more loss of power at higher altitudes), non functional in vanilla (CSP needed?). Default value is 0.1"},
    {"Inertia","Higher value = The engine internals have more inertia (lower revving up and revs will hold for longer)"},
    {"Idle RPM","The RPM at which the engine idles [Integer value]"},
    {"Redline RPM","Max RPM the engine will rev to by itself (Limiter will kick in)"},
    {"Limiter HZ","How often the limiter stops the engine, in Hertz (amount of times per second). Less equals more \"choppy\" limiter bang"},
    {"Coast Reference RPM","The RPM at which coast torque is the given amount"},
    {"Coast Torque","The torque the engine produces while coasting, at the Reference RPM. More = harsher deceleration on lift-off"},
    {"Coast Nonlinearity","How linear the coast curve is. 0 = A line from 0 torque at 0 RPM to Coast Torque at Coast Reference RPM, >0 is some exponential function (value 0 recommended)"},
    {"Turbo Lag Spooling Down","controls the filtering amount for turbo lag when spooling down. 1.0 being fully filtered, 0.0 being unfiltered. Typical values are over 0.980 - 0.990, lag increases dramatically for every 0.01 when nearing 1.0."},
    {"Turbo Lag Spooling Up","controls the filtering amount for turbo lag when spooling up. 1.0 being fully filtered, 0.0 being unfiltered. Typical values are over 0.980 - 0.990, lag increases dramatically for every 0.01 when nearing 1.0."},
    {"Max Boost","Maximum theoretical boost pressure"},
    {"Wastegate Pressure","controls the maximum allowed boost pressure. It is not the real-world wastegate spring pressure or anything similar, but a practical max pressure. Refer to “car physics” app to troubleshoot in-game boost pressure"},
    {"Max Display Boost (for use in game UI)","the boost pressure considered to be maximum by display apps."},
    {"RPM at which max boost is reached","controls the theoretical earliest RPM where MAX_BOOST can be attained, at full throttle with no lag."},
    {"Boost Gamma","controls the gamma function used for the turbo pressure buildup when applying throttle. Exact formula is unknown to the author. Lower gamma = more boost buildup at lower throttle amounts. Typical values are 1.5 - 5.0."},
    {"Boost adjustable from cockpit?", "Controls if turbos are adjustable while driving"},
    {"Turbo damage threshold pressure","controls amount of boost before engine takes damage. Example: 1.10"},
    {"Turbo damage factor","controls damage per second when over threshold. Depends on (boost - threshold). Example: 5"},
    {"Engine damage threshold RPM","controls RPM before engine takes damage. Example: 9000"},
    {"Overrev damage factor","controls engine damage per second. Example: 1"}
};