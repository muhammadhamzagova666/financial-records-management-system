# Financial Records Management System
*Streamline Your Accounting Records with Ease*

## Overview
The Financial Records Management System is a C++ console application designed to simplify the complex process of maintaining accounting records. It enables users to input journal entries, automatically generate ledger accounts, and produce a consolidated trial balance report. This project is ideal for developers and small businesses looking for a lightweight yet powerful solution to manage financial transactions efficiently.

## Technology Stack
- **Language:** C++
- **Platform:** Windows (uses conio.h, Windows.h)
- **Libraries:** iostream, fstream, iomanip
- **Tools:** Visual Studio Code for development and debugging

## Key Features
- **Journal Entry Processing:** Quickly record and format journal entries.
- **Ledger Generation:** Automatically create T-Accounts from journal entries.
- **Trial Balance Report:** Generate trial balance reports to summarize debit and credit totals.
- **User-Friendly CLI:** Interactive prompts guide users through data entry and report generation.

## Installation & Setup
1. **Prerequisites:**
   - A C++ compiler compatible with C++11 or later.
   - Windows operating system.
   - Visual Studio Code for code editing and debugging (optional).

2. **Building the Project:**
   - Open the project folder in Visual Studio Code.
   - Use the integrated terminal and run the following command to compile:
     ```sh
     g++ project.cpp -o FinancialRecordsApp -std=c++11
     ```
   - Ensure that your compiler supports the required Windows-specific headers.

3. **Running the Application:**
   - Execute the generated binary from the terminal:
     ```sh
     ./FinancialRecordsApp
     ```
   - Follow on-screen prompts for journal entry input, ledger creation, and trial balance generation.

## Usage Examples
- **Entering Journal Entries:**
  When prompted, enter journal entries including dates, debit and credit accounts, and amounts.
  
- **Generating Ledger Accounts:**
  After inputting the journal entries, follow the prompts to create ledger accounts for each relevant financial category.
  
- **Viewing Reports:**
  The system outputs the ledger accounts and trial balance reports directly to the console and writes them to `.txt` files for future reference.

## Contribution Guidelines
Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes with clear, concise messages.
4. Submit a pull request detailing your changes.

For major changes, please open an issue first to discuss your ideas.

## Contact
For any questions or suggestions, please contact [muhammadhamzagova666](https://github.com/muhammadhamzagova666).

---
Happy Coding!
