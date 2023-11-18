#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>

using namespace std;

string sourceDir;
string backupDir;
chrono::hours backupInterval;
volatile sig_atomic_t stopDaemon = 0;

void readConfig(const string &configFile) {
    ifstream file(configFile);
    string line;
    string currentSection;

    while (getline(file, line)) {
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        size_t separatorPos = line.find('=');
        if (separatorPos != string::npos) {
            string key = line.substr(0, separatorPos);
            string value = line.substr(separatorPos + 1);

            if (currentSection == "source_directory") {
                if (key == "path")
                    sourceDir = value;
            } else if (currentSection == "backup_directory") {
                if (key == "path")
                    backupDir = value;
            } else if (currentSection == "backup_schedule") {
                if (key == "frequency") {
                    if (value == "daily")
                        backupInterval = chrono::hours(24);
                }
            }
        }
    }
}

void performBackup() {
    try {
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now);
        struct tm *parts = localtime(&now_c);

        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", parts);
        string backupTime(buffer);

        string backupDirWithTime = backupDir + "/backup_" + backupTime;

        filesystem::create_directory(backupDirWithTime);

        for (const auto &entry: filesystem::directory_iterator(sourceDir)) {
            if (filesystem::is_regular_file(entry.path())) {
                filesystem::copy(entry.path(), backupDirWithTime);
            }
        }
    } catch (const exception &e) {
        cerr << "Error during backup: " << e.what() << endl;
    }
}

void handleSignal(int signum) {
    stopDaemon = 1;
}

int main() {
    string configFile = "backup_config.ini";
    readConfig(configFile);

    signal(SIGINT, handleSignal);

    while (!stopDaemon) {
        performBackup();
        cout << "Backup completed successfully.\n";
        this_thread::sleep_for(backupInterval);
    }

    cout << "Stopping the daemon.\n";

    return 0;
}
