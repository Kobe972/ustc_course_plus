/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-14 21:44:05
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-10-31 15:59:54
 * @Description  : file content
 */
#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>

#include <array>
#include <vector>
#include <cmath>
#include <climits>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>

// Boost library
#include <boost/program_options.hpp>

// Application global variables
extern bool gIsSingleStepMode;
extern bool gIsDetailedMode;
extern std::string gInputFileName;
extern std::string gRegisterStatusFileName;
extern std::string gOutputFileName;
extern int gBeginningAddress;