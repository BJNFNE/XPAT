#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <algorithm>

// Set here the versionNumber
const std::string versionNumber = "1.0";

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <script.xcs>\n" << std::endl;
        std::cerr << "Version - " << versionNumber << "\n" << std::endl;
        printf("Supported games:\n");
        printf("Will be added soon");
        printf("<script.xcs>\n\t .xcs script to view\n\n");
        return 1;
    }

    // Here are listed the strings for XCS itself.
    std::string inputXCS = argv[1];

    // for clearing the Console after the Program closed.
    std::string clearConsole;

    // List here everything for the Debug Infos.
    std::string username = getlogin();

    #ifdef _WIN32
        clearConsole = "cls"; // Command to clear the console on Windows
    #else
        clearConsole = "clear"; // Command to clear the console on MacOS/Linux
    #endif

    // Convert the extension to lowercase for case-insensitive comparison
    std::filesystem::path inputScript(inputXCS);
    std::string extension = inputScript.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension != ".xcs") {
        std::cerr << "Error: This File is not an XCS Script!\n" << std::endl;
        return 1;
    }

    // Use the XCS Script for Input
    std::ifstream XCSInput(inputXCS);

    if (!XCSInput) {
        std::cerr << "Error: Unable to find XCS Script." << std::endl;
        return 1;
    }

    // Rewind the XCS Script back to the beginning
    XCSInput.seekg(0);

    // Open the output file for the XCS Script
    std::ofstream XCSOutput(inputScript.stem().string() + ".txt");

    if (!XCSOutput) {
        std::cerr << "Error: Unable to create a text output of the XCS Script." << std::endl;
        return 1;
    }

    // Adds the Date when Output file was created to the Output file
    time_t current_time = time(nullptr);
    char xcs_timedate[100];
    strftime(xcs_timedate, sizeof(xcs_timedate), "%Y-%m-%d %H:%M:%S", localtime(&current_time));

    // Read from input and write to output, keeping track of the offset
    std::streampos offset = XCSInput.tellg();
    char c;
    while (XCSInput.get(c)) {
        if (std::isprint(static_cast<unsigned char>(c))) {
            XCSOutput.put(c);
            offset = XCSInput.tellg(); // Update the offset after each character is processed
        }
    }

    // Close input & output for XCS Script
    XCSInput.close();
    XCSOutput.close();

    // Create a separate file for Debug Infos
    std::ofstream DebugInfoOutput(inputScript.stem().string() + "_debuginfo.txt");
    if (!DebugInfoOutput) {
        std::cerr << "Error: Unable to create Debug Infos file." << std::endl;
        return 1;
    }

    // Write Debug Infos to the separate file
    DebugInfoOutput << "Debug Infos:" << std::endl;
    DebugInfoOutput << "Output of " << inputScript.stem().string() << ".xcs" << " created at " << xcs_timedate << std::endl;
    DebugInfoOutput << "Created by " << username << std::endl;
    DebugInfoOutput << "Offset (hex): 0x" << std::hex << offset << " hex" << std::dec << std::endl;
    DebugInfoOutput << "Offset (bytes): " << offset << " bytes" << std::dec << std::endl;
    DebugInfoOutput.close();

    std::cout << "XCS Script (" << argv[1] << ") is now displayable, and the output is saved to " << inputScript.stem().string() << ".txt" << std::endl;

    // Display the full path of the output file of the XCS Script
    std::cout << "\b" << std::endl;
    printf("Output file created at: %s\n", std::filesystem::absolute(inputScript.stem().string() + ".txt").c_str());

    // Exit message for XCS Viewer
    std::cout << "\b" << std::endl;
    std::cout << "Press Enter to exit XCS Viewer & clear the Console" << std::endl;
    getchar();
    system(clearConsole.c_str());

    return 0;
}