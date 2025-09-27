#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <ctime>

using namespace std;

// Enum for log levels - better than static integers
enum class LogLevel {
    INFO = 1,
    DEBUG = 2,
    WARNING = 3,
    ERROR = 4,
    FATAL = 5
};

// Convert log level to string for display
string logLevelToString(LogLevel level) {
    switch(level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

// Get current timestamp
string getCurrentTimestamp() {
    time_t now = time(0);
    char* timeStr = ctime(&now);
    string result(timeStr);
    result.pop_back(); // Remove newline character
    return result;
}

/**
 * CHAIN OF RESPONSIBILITY DESIGN PATTERN
 * 
 * This is a behavioral design pattern that lets you pass requests along a chain of handlers.
 * Upon receiving a request, each handler decides either to process the request or to pass it 
 * to the next handler in the chain.
 * 
 * Key Components:
 * 1. Handler (Abstract Logger): Defines interface for handling requests and optional next handler
 * 2. Concrete Handlers (InfoLogger, DebugLogger, etc.): Handle specific types of requests
 * 3. Client: Initiates the request to the chain
 * 
 * Benefits:
 * - Decouples sender and receiver
 * - Allows dynamic chain composition
 * - Single Responsibility Principle
 * - Open/Closed Principle - can add new handlers without changing existing code
 */

// Abstract Base Logger Class (Handler in Chain of Responsibility)
class Logger {
protected:
    LogLevel level;
    shared_ptr<Logger> nextLoggerInChain;  // Next handler in chain
    
public:
    Logger(LogLevel logLevel) : level(logLevel), nextLoggerInChain(nullptr) {}
    
    // Set the next logger in chain
    void setNextLogger(shared_ptr<Logger> nextLogger) {
        nextLoggerInChain = nextLogger;
    }
    
    // Main method that implements Chain of Responsibility
    void logMessage(LogLevel msgLevel, const string& message) {
        // If current logger can handle this level, process it
        if (msgLevel >= level) {
            write(message);
        }
        
        // Always pass to next logger in chain (if exists)
        if (nextLoggerInChain != nullptr) {
            nextLoggerInChain->logMessage(msgLevel, message);
        }
    }
    
protected:
    // Pure virtual method - each concrete logger implements its own way
    virtual void write(const string& message) = 0;
    
    // Helper method to format log message
    virtual string formatMessage(const string& message, LogLevel msgLevel) {
        return "[" + getCurrentTimestamp() + "] [" + 
               logLevelToString(msgLevel) + "] " + message;
    }
};

// Concrete Logger 1: Console Logger
class ConsoleLogger : public Logger {
public:
    ConsoleLogger(LogLevel level) : Logger(level) {}
    
protected:
    void write(const string& message) override {
        cout << "CONSOLE: " << formatMessage(message, level) << endl;
    }
};

// Concrete Logger 2: File Logger
class FileLogger : public Logger {
private:
    string filename;
    
public:
    FileLogger(LogLevel level, const string& file) : Logger(level), filename(file) {}
    
protected:
    void write(const string& message) override {
        ofstream file(filename, ios::app);  // Append mode
        if (file.is_open()) {
            file << "FILE: " << formatMessage(message, level) << endl;
            file.close();
            cout << "Message logged to file: " << filename << endl;
        } else {
            cerr << "Error: Could not open file " << filename << endl;
        }
    }
};

// Concrete Logger 3: Error Logger (for critical errors)
class ErrorLogger : public Logger {
public:
    ErrorLogger(LogLevel level) : Logger(level) {}
    
protected:
    void write(const string& message) override {
        cerr << "ERROR LOG: " << formatMessage(message, level) << endl;
        // Could also send email, SMS, or other critical notifications
        cout << "*** CRITICAL ERROR ALERT SENT ***" << endl;
    }
};

// Concrete Logger 4: Debug Logger (detailed debugging info)
class DebugLogger : public Logger {
public:
    DebugLogger(LogLevel level) : Logger(level) {}
    
protected:
    void write(const string& message) override {
        cout << "DEBUG: " << formatMessage(message, level) << " [PID: " << 
               getpid() << "]" << endl;
    }
    
private:
    int getpid() {
        // Simulate process ID
        return 12345;
    }
};

// Logger Builder/Factory to create the chain
class LoggerChainBuilder {
public:
    static shared_ptr<Logger> createLoggerChain() {
        // Create individual loggers
        auto infoLogger = make_shared<ConsoleLogger>(LogLevel::INFO);
        auto debugLogger = make_shared<DebugLogger>(LogLevel::DEBUG);
        auto fileLogger = make_shared<FileLogger>(LogLevel::WARNING, "app.log");
        auto errorLogger = make_shared<ErrorLogger>(LogLevel::ERROR);
        
        // Build the chain: INFO -> DEBUG -> WARNING(File) -> ERROR
        infoLogger->setNextLogger(debugLogger);
        debugLogger->setNextLogger(fileLogger);
        fileLogger->setNextLogger(errorLogger);
        
        return infoLogger;  // Return the first logger in chain
    }
    
    // Alternative chain for production (less verbose)
    static shared_ptr<Logger> createProductionLoggerChain() {
        auto fileLogger = make_shared<FileLogger>(LogLevel::WARNING, "production.log");
        auto errorLogger = make_shared<ErrorLogger>(LogLevel::ERROR);
        
        fileLogger->setNextLogger(errorLogger);
        return fileLogger;
    }
};

// Enhanced Logger Manager (Singleton pattern)
class LoggerManager {
private:
    shared_ptr<Logger> loggerChain;
    static LoggerManager* instance;
    
    LoggerManager() {
        loggerChain = LoggerChainBuilder::createLoggerChain();
    }
    
public:
    static LoggerManager* getInstance() {
        if (instance == nullptr) {
            instance = new LoggerManager();
        }
        return instance;
    }
    
    void info(const string& message) {
        loggerChain->logMessage(LogLevel::INFO, message);
    }
    
    void debug(const string& message) {
        loggerChain->logMessage(LogLevel::DEBUG, message);
    }
    
    void warning(const string& message) {
        loggerChain->logMessage(LogLevel::WARNING, message);
    }
    
    void error(const string& message) {
        loggerChain->logMessage(LogLevel::ERROR, message);
    }
    
    void fatal(const string& message) {
        loggerChain->logMessage(LogLevel::FATAL, message);
    }
    
    // Method to switch to different chain configurations
    void useProductionChain() {
        loggerChain = LoggerChainBuilder::createProductionLoggerChain();
    }
};

// Static member definition
LoggerManager* LoggerManager::instance = nullptr;

// Demo Application
class Application {
public:
    void simulateApplicationFlow() {
        LoggerManager* logger = LoggerManager::getInstance();
        
        cout << "=== Application Starting ===" << endl;
        logger->info("Application started successfully");
        
        cout << "\n=== Processing User Request ===" << endl;
        logger->debug("Processing user login request");
        logger->info("User authentication successful");
        
        cout << "\n=== Warning Scenario ===" << endl;
        logger->warning("Database connection pool is 80% full");
        
        cout << "\n=== Error Scenario ===" << endl;
        logger->error("Failed to connect to payment gateway");
        
        cout << "\n=== Fatal Error Scenario ===" << endl;
        logger->fatal("System out of memory - shutting down");
        
        cout << "\n=== Switching to Production Mode ===" << endl;
        logger->useProductionChain();
        logger->info("This info message won't appear in production mode");
        logger->error("This error will still be logged in production");
    }
};

int main() {
    cout << "======================================" << endl;
    cout << "    LOGGER SYSTEM DEMONSTRATION       " << endl;
    cout << "  Chain of Responsibility Pattern     " << endl;
    cout << "======================================" << endl;
    
    // Demonstrate the logger system
    Application app;
    app.simulateApplicationFlow();
    
    cout << "\n======================================" << endl;
    cout << "         Pattern Explanation          " << endl;
    cout << "======================================" << endl;
    
    cout << "\nChain of Responsibility Pattern in Logger System:" << endl;
    cout << "1. Each logger handles specific log levels" << endl;
    cout << "2. Messages pass through the chain sequentially" << endl;
    cout << "3. Multiple loggers can process the same message" << endl;
    cout << "4. Easy to add/remove loggers without changing existing code" << endl;
    cout << "5. Decouples the sender from receivers" << endl;
    
    cout << "\nLogger Chain Flow:" << endl;
    cout << "Request -> ConsoleLogger -> DebugLogger -> FileLogger -> ErrorLogger" << endl;
    
    cout << "\nBenefits Demonstrated:" << endl;
    cout << "✓ Single Responsibility: Each logger has one job" << endl;
    cout << "✓ Open/Closed: Can add new loggers without modifying existing ones" << endl;
    cout << "✓ Flexible Configuration: Different chains for different environments" << endl;
    cout << "✓ Runtime Chain Modification: Can switch chains dynamically" << endl;
    
    return 0;
}