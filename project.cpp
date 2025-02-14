// =============================================================================
// Financial Records Management System
// This file implements journal entry processing and ledger trial generation.
// Target Users: Developers maintaining an accounting/financial records system.
// Code Style: Follows Doxygen conventions for documentation and concise inline comments.
// =============================================================================

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include <ctime>

using namespace std;

// Global array to store ledger file names for later retrieval.
string ledgernames[50] = {""};    
int index = 0;

/**
 * @class journal
 * @brief Handles user input for journal entries and writes them to a file.
 *
 * This class enables users to input debit and credit entries for journal records.
 * It formats and writes these entries into "journal.txt" for later processing.
 */
class journal
{
private:
    string from;           ///< Debit account name/input.
    string to;             ///< Credit account name/input.
    string date;           ///< Date of the entry.
    long long int amount;  ///< Transaction amount.
    ofstream in;           ///< Output file stream for journal file.
    int count;             ///< Counter for the number of entries.

public:
    /**
     * @brief Default constructor that initializes the journal entry variables.
     */
    journal() : count(0), from(""), to(""), amount(0) {}

    /**
     * @brief Destructor.
     */
    ~journal() {}

    /**
     * @brief Prompts the user to input multiple journal entries.
     *
     * Opens "journal.txt", displays the header/logo,
     * then repeatedly accepts debit and credit entries along with a description.
     * A loop continues until the user chooses to exit.
     */
    void get_entries()
    {
        in.open("journal.txt");
        display_logo(); // Print the header with journal name and date.
        int i = 0;
        while (i == 0)
        {
            ++count;
            in << endl;
            system("cls");
            // Capture debit details.
            debit();

            // Capture credit details.
            credit();

            // Prompt for descriptive information about the journal entry.
            string disc;
            cout << "\n\n\tPlease enter a description for the journal entry: ";
            cin.ignore();
            getline(cin, disc);
            in << setw(11) << "|(" << setw(50) << disc << ")" << endl;

            // Add a horizontal divider for better readability.
            for (int j = 125; j > 0; j--)
            {
                in << "-";
            }

            // Ask user if they wish to exit or continue entering data.
            cout << "\n\n\n\n\tEnter 1 to exit or 0 to continue: ";
            cin >> i;
            system("cls");
        }
        in.close();
    }

    /**
     * @brief Captures credit-side details from the user.
     *
     * Writes formatted credit data into the journal entry file.
     */
    void credit()
    {
        in << setw(10) << "|";
        cout << "\n\n\tEnter the credit account name: ";
        cin >> to;
        in << setw(40) << "to " << to;
        cout << "\n\n\tEnter the credit amount: ";
        cin >> amount;
        in << setw(50) << amount << endl;
    }

    /**
     * @brief Captures debit-side details from the user.
     *
     * Retrieves the date, debit account name, and debit amount,
     * and writes the formatted entry into "journal.txt".
     */
    void debit()
    {
        cout << "\n\n\tEnter the date of the entry (DD/MM/YYYY): ";
        cin.clear();
        // Get the date input; if already entered previously, skip extra newline.
        getline(cin, date);
        if (count > 1)
        {
            getline(cin, date);
        }
        in << date;
        cout << "\n\n\tEnter the debit account name: ";
        cin.clear();
        getline(cin, from);
        in << setw(20) << from;
        cout << "\n\n\tEnter the debit amount: ";
        cin >> amount;
        in << setw(70) << amount << endl;
    }

    /**
     * @brief Displays the journal header with project title and date.
     *
     * This function prints a professional header to the journal file,
     * setting the context for the entries that follow.
     */
    void display_logo()
    {
        string name;
        cout << "\n\n\tEnter the journal name/identifier: ";
        cin.ignore();
        cin.clear();
        getline(cin, name);
        cout << "\n\n\tEnter the journal date (DD/MM/YYYY): ";
        getline(cin, date);

        in << setw(50) << name << endl;
        in << setw(50) << "JOURNAL" << endl;
        in << setw(50) << date << endl;
        // Print a divider line.
        for (int j = 125; j > 0; j--)
        {
            in << "-";
        }
        in << endl;
        in << setw(10) << "DATE|" << setw(25) << "DESCRIPTION" 
           << setw(50) << "|DEBIT" << setw(20) << "|CREDIT" << endl;
        for (int j = 125; j > 0; j--)
        {
            in << "-";
        }
    }
};


// Global variables to accumulate debits and credits for trial balance.
long long int trial_debit_sum = 0, trial_credit_sum = 0;            

/**
 * @class ledger_trial
 * @brief Processes journal entries to generate ledger accounts and a trial balance.
 *
 * This class reads the "journal.txt" file to extract entries for a specific ledger,
 * formats the ledger report, and also updates a summary trial balance in "Trial.txt".
 */
class ledger_trial
{
private:
    ofstream out_file;          ///< Output stream for the ledger file.
    ofstream out_file_trial;    ///< Output stream for the trial balance file.
    ifstream in_file;           ///< Input stream to read from journal file.
    string filename;            ///< Ledger filename (without extension).
    string account_name;        ///< Name of the account extracted from the journal.
    long long int debit_sum;    ///< Accumulated debit amount for the ledger.
    long long int credit_sum;   ///< Accumulated credit amount for the ledger.
    string entery;            ///< Debit entry account name from the journal.
    string date;              ///< Date of the ledger entry.
    string entry;             ///< Credit entry account name from the journal.
    long long int amount;     ///< Debit entry amount.
    long long int amount1;    ///< Credit entry amount.
    long long int balance_cd; ///< Calculated balance (dr or cr) for the ledger.
    bool flag = true;         ///< Indicates if the balance is on the debit side.

public:
    /**
     * @brief Constructor that initializes ledger sums and opens the trial file.
     */
    ledger_trial()
    {
        debit_sum = credit_sum = 0;
        trial_credit_sum = trial_debit_sum = 0;
        balance_cd = 0;
        // Append to trial balance report file.
        out_file_trial.open("Trial.txt", ios::app);
    }

    /**
     * @brief Destructor.
     */
    ~ledger_trial() {}

    /**
     * @brief Initiates the creation of a ledger account.
     *
     * Prompts the user for a ledger name, creates a corresponding ledger file,
     * and writes the header information before reading relevant journal entries.
     */
    void start()
    {
        string ledger_name; // Used to form the ledger file name.
        debit_sum = credit_sum = 0;
        balance_cd = 0;    // Reset previous ledger values.
        filename = account_name = entery = date = entry = "";
        cout << "\n\n\n\n\tEnter the ledger name: ";
        cin >> filename;

        ledger_name = filename + ".txt";  // Append ".txt" extension.
        ledgernames[index++] = ledger_name;

        out_file.open(ledger_name.c_str());
        in_file.open("journal.txt"); // Open journal to search for matching entries.

        // Read and print the header from the journal file.
        getline(in_file, account_name);
        out_file << account_name << endl;
        out_file << setw(50) << filename << endl;
        out_file << setw(50) << "LEDGER" << endl;

        // Print a horizontal divider in the ledger.
        for (int j = 86; j > 0; j--)
        {
            out_file << "-";
        }
        out_file << endl;
        out_file << "Date" << setw(20) << "Particular";
        out_file << setw(15) << "Amount" 
                 << setw(10) << "Date" << setw(20) << "Particular" 
                 << setw(15) << "Amount" << endl;
        for (int j = 86; j > 0; j--)
        {
            out_file << "-";
        }
        out_file << endl;
        get_entries();  // Process and write journal entries for this ledger.
    }

    /**
     * @brief Reads journal file entries and filters those matching the ledger.
     *
     * Extracts debit and credit entries from the journal, formats them into the ledger,
     * and sums up totals on both sides.
     */
    void get_entries()
    {
        string waste;  // Temporary variable to skip unneeded parts of each journal entry.
        for (int i = 0; i < 5; i++)
            getline(in_file, waste);  // Skip header lines not relevant to entries.

        do
        {
            // Read a debit entry.
            in_file >> date >> entery >> amount;
            // Skip intermediary text tokens.
            for (int i = 0; i < 2; i++)
                in_file >> waste;
            // Read the corresponding credit entry.
            in_file >> entry >> amount1;
            // Based on comparison, decide on debit or credit entry for this ledger.
            if (entery == filename)
            {
                out_file << setw(10) << date 
                         << setw(20) << entery 
                         << setw(10) << amount1 << "|" << endl;
                debit_sum += amount;  // Sum debit entries.
            }
            else if (entry == filename)
            {
                out_file << setw(41) << "|" 
                         << setw(9) << date 
                         << setw(20) << entry 
                         << setw(15) << amount << endl;
                credit_sum += amount1;  // Sum credit entries.
            }
            // Skip any trailing lines associated with the entry.
            for (int i = 0; i < 3; i++)
            {
                getline(in_file, waste);
            }
        } while (!in_file.eof());
        total();  // Finalize the ledger by printing totals and closing files.
        trial();
    }

    /**
     * @brief Finalizes the ledger by calculating the balance and formatting totals.
     *
     * Depending on whether the ledger is in debit or credit, this function prints
     * the balance along with a horizontal line for separation.
     */
    void total()
    {
        if (debit_sum > credit_sum)
        {
            flag = true;
            balance_cd = debit_sum - credit_sum;
            for (int j = 86; j > 0; j--)
            {
                out_file << "-";
            }
            out_file << endl;
            out_file << setw(10) << "Total" 
                     << setw(10) << (debit_sum - credit_sum) << endl;
            for (int j = 86; j > 0; j--)
            {
                out_file << "-";
            }
            out_file << endl;
        }
        else
        {
            flag = false;
            balance_cd = credit_sum - debit_sum;
            for (int j = 86; j > 0; j--)
            {
                out_file << "-";
            }
            out_file << endl;
            out_file << setw(55) << "Total " 
                     << setw(15) << (credit_sum - debit_sum) << endl;
            for (int j = 86; j > 0; j--)
            {
                out_file << "-";
            }
            out_file << endl;
            out_file.close();
            in_file.close();
        }
    }

    /**
     * @brief Designs the trial balance header from the journal file.
     *
     * This creates a standardized header in the trial balance report,
     * ensuring consistency in the recorded financial summaries.
     */
    void design()
    {
        string name_of_account, date_of_trial;
        ifstream in_file_design;
        in_file_design.open("journal.txt", ios::in);
        getline(in_file_design, name_of_account);

        out_file_trial << name_of_account << endl;
        out_file_trial << setw(50) << "TRIAL" << endl;

        getline(in_file_design, date_of_trial);
        getline(in_file_design, date_of_trial);

        out_file_trial << setw(50) << date_of_trial << endl;
        in_file_design.close();

        // Print divider for the trial balance header.
        for (int j = 50; j > 0; j--)
        {
            out_file_trial << "-";
        }
        out_file_trial << endl;
        out_file_trial << setw(10) << "Ledger name" 
                       << setw(15) << "DR Amount" 
                       << setw(15) << "CR Amount" << endl;
        for (int j = 50; j > 0; j--)
        {
            out_file_trial << "-";
        }
        out_file_trial << endl;
    }

    /**
     * @brief Updates the trial balance file with the current ledger's balance.
     *
     * Determines if the balance is a debit or credit and adds it to the trial totals.
     */
    void trial()
    {
        if (flag == true)
        {
            trial_debit_sum += balance_cd;
            out_file_trial << setw(10) << filename 
                           << setw(15) << balance_cd << endl;
        }
        else
        {
            trial_credit_sum += balance_cd;
            out_file_trial << setw(10) << filename 
                           << setw(30) << balance_cd << endl;
        }
    }

    /**
     * @brief Finalizes the trial balance report by printing total amounts.
     *
     * Completes the trial balance with a final divider and the sum of all ledgers.
     */
    void total_trial()
    {
        for (int j = 50; j > 0; j--)
        {
            out_file_trial << "-";
        }
        out_file_trial << endl;
        out_file_trial << setw(10) << "TOTAL" 
                       << setw(15) << trial_debit_sum 
                       << setw(15) << trial_credit_sum << endl;
        for (int j = 50; j > 0; j--)
        {
            out_file_trial << "-";
        }
        out_file_trial << endl;
        out_file_trial.close();
    }
};

/**
 * @brief Main routine for the Financial Records Management System.
 *
 * Displays a welcome message, provides loading feedback,
 * and coordinates the workflow for journal entry and ledger/trial balance generation.
 */
int main()
{
    system("Color 0F");
    int oneSecond = 1000;

    // Display the project banner.
    cout << "\t**      **      ";
    cout << "\n\t**      ** ******* **      ****** ******** ****** *** ***********";
    cout << "\n\t**      ** **      **      **     **    ** **  **  ** ***      ";
    cout << "\n\t**  **  ** *****   **      **     **    ** **  **  ** **********    ";
    cout << "\n\t**  **  ** **      **      **     **    ** **  **  ** ***             ";
    cout << "\n\t********** ******* ******* ****** ******** **      ** ************     ";
    cout << "\n\n\t\t\t        ********  *****        ";
    cout << "\n\t\t\t           **     *   *      ";
    cout << "\n\t\t\t           **     *****    ";
    cout << "\n\n\t -------------------- Inventory Management System -------------------------";

    // Display system-generated date and time.
    char date[10];
    _strdate(date);
    char time[10];
    _strtime(time);
    cout << "\n\n\t\tDate: " << date << "  Time: " << time << endl;
    cout << "\n\n\n\tLoading data, please wait...";

    // Provide visual loading feedback.
    cout << "\n\tPlease wait";
    for (int i = 0; i < 5; i++)
    {
        Sleep(500);
        cout << ". ";
    }
    Sleep(1000);
    system("pause");
    system("cls");

    // Journal entry phase.
    cout << "\n\n\n\tPress '1' to enter journal entries for the first time: ";
    int number;
    cin >> number;
    journal j;
    if (number == 1)
        j.get_entries();

    system("cls");

    string printer_of_journal_ledger;
    cout << "\n\n\n\tEnter 1 to display your journal entries, else 0: ";
    cin >> number;
    system("cls");
    if (number == 1)
    {
        ifstream infile("journal.txt", ios::in);
        do
        {
            getline(infile, printer_of_journal_ledger);
            cout << printer_of_journal_ledger << endl;
        } while (!infile.eof());
        infile.close();
        cout << endl;
        for (int i = 0; i < 5; i++)
        {
            Sleep(oneSecond);
            cout << ". ";
        }
    }

    // Process ledger/ trial accounts.
    ledger_trial t[50];
    t[0].design();
    system("cls");

    int i;
    for (i = 1; i < 50; i++)
    {
        cout << "\n\n\n\tEnter 1 to add a new ledger account (T-Account), else 0: ";
        cin >> number;
        system("cls");
        if (number == 0)
            break;
        t[i].start();
    }

    // Finalize the trial balance report.
    t[i].total_trial();

    cout << "\n\n\n\tEnter 1 to view your ledger accounts, else 0: ";
    cin >> number;
    if (number == 0)
        exit(0);

    // Display each ledger file to the user.
    ifstream infile;
    for (int i = 0; i < index; i++)
    {
        infile.open(ledgernames[i].c_str(), ios::in);
        do
        {
            getline(infile, printer_of_journal_ledger);
            cout << printer_of_journal_ledger << endl;
        } while (!infile.eof());
        infile.close();
        cout << "\n\n\n";
    }
    for (int i = 0; i < 5; i++)
    {
        Sleep(oneSecond);
        cout << ". ";
    }
}