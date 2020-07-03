#include <iostream>
#include <string>
#include <limits>
#include "magsv.h"

using namespace std;

const double IB_MIN = 0., IB_MAX = 180.;
const double VB_MIN = 0., VB_MAX = 1300.;
const double PHIB_MIN = -125., PHIB_MAX = 125.;
const double IAUX_LOW_MIN = -125., IAUX_LOW_MAX = 125.;
const double IAUX_HIGH_MIN = -500., IAUX_HIGH_MAX = 500.;



void errorout(unsigned short error) {
    if (error != 0)
        cout << "\nCommunication Error" << endl;
}



// Check user's command-line arguments for general validity
// Check the number of arguments given, and if appropriate, open USB connection and set the active channel.
// Arguments: an unsigned short array with three elements to store channel, active, and error;
// argc and argv from the command-line arguments; the function call in the form "*.exe"; the number of
// extra arguments in addition to the call, channel, whether to set active, baud, and timeout (i.e. argc minus 5); 
// the extra argument descriptions.

void validate_args(unsigned short* containers, int argc, char** argv, const char* exe_name, int n_extra_args, const char** extra_args) {

    // Check number of arguments
    if (argc != (3 + n_extra_args) && argc != (5 + n_extra_args)) {
        cout << "ERROR: run the program as follows" << endl;
        printf("\t.\\%s channel[1,2,3] active[0,1] ", exe_name);
        for (int i = 0; i < n_extra_args; i++) {
            printf("%s ", extra_args[i]);
        }
        cout << "[baud = 57600] [timeout = 100]" << endl;
        cout << "\tNote: either set both baud and timeout or neither." << endl;
        cout << "\tNote: active indicates whether to make the channel the active channel." << endl;
        cout << flush;
        return;
    }

    // Set connection info
    unsigned long baud = 57600, timeout = 100; 
    if (argc == (5 + n_extra_args)) {
        baud = stoul(argv[3 + n_extra_args]);
        timeout = stoul(argv[4 + n_extra_args]);
    }

    // Set containers: channel, whether to set active, error
    unsigned short channel = (unsigned short)(stoul(argv[1]));
    unsigned short active = (unsigned short)(stoul(argv[2]));
    unsigned short error = 0;

    // Connect to electronics
    MA_initUSB(&error, baud, timeout);
    errorout(error);

    // Set active channel if desired
    if (active == 1) {
        MA_SetActiveChannel(channel, &error);
        errorout(error);
    }

    // Return containers
    containers[0] = channel;
    containers[1] = active;
    containers[2] = error;
}
