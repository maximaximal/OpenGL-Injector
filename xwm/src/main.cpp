/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *  Copyright (C) 2013-2017 Chuan Ji <ji@chu4n.com>                          *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *   http://www.apache.org/licenses/LICENSE-2.0                              *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <easyloggingpp/easylogging++.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <memory>
#include <piga/xwm/WindowManager.hpp>

#include <sys/stat.h>

INITIALIZE_EASYLOGGINGPP

using ::std::unique_ptr;

// Source: http://stackoverflow.com/a/12774387
inline bool fileExists(const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

void forkAndExec(const std::string &cmd) {
    pid_t pid = fork();

    if(pid == 0) {
	const char* hold = strdup(cmd.c_str());
	char* argv[2]; 

	argv[0] = const_cast<char*>(strdup(cmd.c_str()));
	argv[1] = nullptr;

	execvp(hold, argv);
    }
}

void startOverlay() {
    if(fileExists("../overlay/piga-overlay-wrapper")) {
	// This file marks the current debugging directory. Use this.
	forkAndExec("../overlay/piga-overlay-wrapper");
    }
    else {
	// Just try for the best. This variant is a fallback and requires the wrapper of the
	// overlay to function correctly.
	forkAndExec("piga-overlay-wrapper");
    }
}

int main(int argc, char** argv) {
    START_EASYLOGGINGPP(argc, argv);
    
    unique_ptr<WindowManager> window_manager = WindowManager::Create();
    if (!window_manager) {
	return EXIT_FAILURE;
    }

    startOverlay();
    
    window_manager->Run(argc, argv);

    return EXIT_SUCCESS;
}
