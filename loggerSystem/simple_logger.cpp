#include <iostream>
#include <string>
using namespace std;

/**
 * SIMPLE CHAIN OF RESPONSIBILITY PATTERN - LOGGER SYSTEM
 * 
 * Chain: InfoLogger -> DebugLogger -> ErrorLogger
 * 
 * How it works:
 * 1. Each logger checks if it can handle the message level
 * 2. If yes, it processes the message  
 * 3. Then passes the message to next logger in chain
 * 4. This continues until end of chain
 */

// Base Logger class
class Logger {
protected:
    int level;           // Logger level (1=INFO, 2=DEBUG, 3=ERROR)
    Logger* nextLogger;  // Next logger in chain
    
public:
    Logger(int level) {
        this->level = level;
        this->nextLogger = nullptr;
    }
    
    // Set next logger in chain
    void setNext(Logger* next) {
        nextLogger = next;
    }
    
    // Main method - processes or passes to next logger
    void logMessage(int msgLevel, string message) {
        // If this logger can handle this level, process it
        if (msgLevel >= this->level) {
            write(message);
        }
        
        // Always pass to next logger if it exists
        if (nextLogger != nullptr) {
            nextLogger->logMessage(msgLevel, message);
        }
    }
    
    // Pure virtual - each logger implements its own way
    virtual void write(string message) = 0;
};

// INFO Logger - prints to console
class InfoLogger : public Logger {
public:
    InfoLogger() : Logger(1) {}  // Level 1 = INFO
    
    void write(string message) override {
        cout << "[INFO] " << message << endl;
    }
};

// DEBUG Logger - prints with debug format
class DebugLogger : public Logger {
public:
    DebugLogger() : Logger(2) {}  // Level 2 = DEBUG
    
    void write(string message) override {
        cout << "[DEBUG] " << message << endl;
    }
};

// ERROR Logger - prints errors with special format
class ErrorLogger : public Logger {
public:
    ErrorLogger() : Logger(3) {}  // Level 3 = ERROR
    
    void write(string message) override {
        cout << "[ERROR] *** " << message << " ***" << endl;
    }
};

// Helper function to create the logger chain
Logger* createLoggerChain() {
    // Create individual loggers
    Logger* infoLogger = new InfoLogger();
    Logger* debugLogger = new DebugLogger();  
    Logger* errorLogger = new ErrorLogger();
    
    // Build the chain: INFO -> DEBUG -> ERROR
    infoLogger->setNext(debugLogger);
    debugLogger->setNext(errorLogger);
    
    return infoLogger;  // Return first logger in chain
}

// Simple demonstration
void demonstrateLogging() {
    cout << "=== Simple Logger Chain Demo ===" << endl;
    
    // Create the logger chain
    Logger* loggerChain = createLoggerChain();
    
    cout << "\n1. Sending INFO message (level 1):" << endl;
    loggerChain->logMessage(1, "User logged in successfully");
    
    cout << "\n2. Sending DEBUG message (level 2):" << endl;
    loggerChain->logMessage(2, "Database connection established");
    
    cout << "\n3. Sending ERROR message (level 3):" << endl;
    loggerChain->logMessage(3, "Connection timeout occurred");
    
    // Show what happens with different levels
    cout << "\n=== Understanding the Chain ===" << endl;
    cout << "When we send INFO (level 1):" << endl;
    loggerChain->logMessage(1, "This appears in all loggers");
    
    cout << "\nWhen we send DEBUG (level 2):" << endl;
    loggerChain->logMessage(2, "This appears in DEBUG and ERROR loggers only");
    
    cout << "\nWhen we send ERROR (level 3):" << endl;
    loggerChain->logMessage(3, "This appears in ERROR logger only");
    
    // Cleanup (simplified - just delete the first one for demo)
    delete infoLogger;
    delete debugLogger;
    delete errorLogger;
}

int main() {
    cout << "Chain of Responsibility Pattern - Logger System" << endl;
    cout << "==============================================" << endl;
    
    demonstrateLogging();
    
    cout << "\n=== Pattern Explanation ===" << endl;
    cout << "Chain: InfoLogger -> DebugLogger -> ErrorLogger" << endl;
    cout << "\n1. Each logger handles messages >= its level" << endl;
    cout << "2. INFO logger (level 1) handles all messages (1, 2, 3)" << endl;
    cout << "3. DEBUG logger (level 2) handles DEBUG and ERROR (2, 3)" << endl;
    cout << "4. ERROR logger (level 3) handles only ERROR (3)" << endl;
    cout << "5. All loggers in chain get a chance to process the message" << endl;
    
    cout << "\n=== Key Benefits ===" << endl;
    cout << "✓ Loose coupling - sender doesn't know which logger will handle" << endl;
    cout << "✓ Dynamic chain - can add/remove loggers at runtime" << endl;
    cout << "✓ Single responsibility - each logger has one job" << endl;
    cout << "✓ Flexible - easy to add new logger types" << endl;
    
    return 0;
}