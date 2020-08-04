#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <dirent.h>

static std::vector<std::string> dirvect;
/* accrun compiles and executes via Alex's C Compiler, then stores
 * the return code in the respective file to be compared against GCC.
 */
void accrun(std::string file) {
    // Compile the program
    std::string testfile("./compiler gcctests/");
    testfile.append(file);
    system(testfile.c_str());
    
    // Link the program
    system("gcc -o test test.o");
    
    // Execute the program; store return code in the respective results file
    system("./test; echo $? >> accresults.txt");
}

/* gccrun compiles and executes via Gnu's C Compiler, then stores
 * the return code in the respective file to be compared against ACC.
 */
void gccrun(std::string file) {
    // Compile and link the program
    std::string testfile("gcc -o test gcctests/");
    testfile.append(file);
    system(testfile.c_str());
    
    // Execute the program; store return code in the respective results file
    system("./test; echo $? >> gccresults.txt");
}

int main() {
    
    // Let the user know what's going on.
    printf("Running Alex's C Compiler (ACC) vs Gnu C Compiler (GCC) for the following files:\n");
    
    std::string testdir("./gcctests");
    
    DIR* dir = opendir(testdir.c_str());
    dirent* d;
    
    // Populate the directory vector
    if(dir) {
        while (d = readdir(dir)) {
            if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
                continue;
            dirvect.push_back(d->d_name);
        }
    }
    
    // Print a list of all the files we'll be testing against.
    printf("FileNo\tFileName\n");
    for(int i = 0; i < dirvect.size(); i++) {
        printf("  %d\t%s\n", i, dirvect[i].c_str());
    }
    printf("Press any key to continue...");
    std::cin.get();
    
    // In case the results didn't get wiped after last test, wipe now
    // to ensure a clean test run
    system("rm -f accresults.txt gccresults.txt");
    
    // compile and run the programs, and store outputs.
    // ACC gets stored in accresults.txt
    // GCC gets stored in gccresults.txt
    // both of which are deleted post-tests, but the results
    // are printed to the screen
    for(int i = 0; i < dirvect.size(); i++){
        accrun(dirvect[i]);
        gccrun(dirvect[i]);
    }
    
    // Make the pretty stuff
    int accresults[dirvect.size()] = {};
    int gccresults[dirvect.size()] = {};
    
    // Populate the result arrays with the contents of the respective
    // result files.
    std::fstream accfile;
    accfile.open("accresults.txt");
    int number;
    int idx = 0;
    while(accfile >> number) {
        accresults[idx] = number;
        idx++;
    }
    accfile.close();
    
    std::fstream gccfile;
    gccfile.open("gccresults.txt");
    idx = 0;
    while(gccfile >> number) {
        gccresults[idx] = number;
        idx++;
    }
    gccfile.close();
    
    // Done with the result txt files -- clean up (delete them)
    system("rm -f accresults.txt gccresults.txt test test.o");
    
    // Print the pretty stuff out.
    printf("\n");
    printf("o===================== TEST RESULTS =====================o\n");
    printf("FileNo\tACCResult GCCResult\tPass?\tFileName\n");
    for(int i = 0; i < dirvect.size(); i++){
        printf("  %d\t   %d\t     %d\t\t", i, accresults[i], gccresults[i]);
        if(accresults[i] == gccresults[i])
            printf("\u001b[32mPASS\t\u001b[0m");
        else
            printf("\u001b[31mFAIL\t\u001b[0m");
        printf("%s\n", dirvect[i].c_str());
    }
    printf("o========================================================o\n");
    
}