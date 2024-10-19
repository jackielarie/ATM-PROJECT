# ATM Program with Flash Drive Detection

# Overview
This program simulates an ATM system that stores account information on a removable USB flash drive. It detects when a flash drive is inserted and automatically creates a file (`ATM_CARD.txt`) to store the account data, ensuring the data persists even after exiting the program.

# Features
- Detects if a USB flash drive is inserted.
- Automatically creates a text file (`ATM_CARD.txt`) on the flash drive to store account information.
- Reads account data from the file when the flash drive is inserted.
- Updates the account file with new transactions or account changes.
- Ensures data persistence across program exits.

# Usage
1. Insert the Flash Drive:
   The program requires a USB flash drive to be inserted to proceed with the ATM functionalities. It will detect if a drive is inserted, and if not, it will prompt the user to insert a drive.
2. Account Data:
   The program creates a file named `ATM_CARD.txt` on the flash drive to store account information such as account number, PIN, and balance.
   - The file format for account data is:
     <AccountName>,<AccountNumber>,<PINCode>,<Balance>
     Example:
     John Lee,12345,1234,5000
3. Reading and Writing Data:
   - When an account is added or updated, the program writes the data to the `ATM_CARD.txt` file.
   - The program reads all the account data from the `ATM_CARD.txt` file whenever the flash drive is inserted.

# Key Functions
- **isCardInserted()**:
  - Detects if a flash drive is inserted by scanning all available drives on the system.
  - Checks if the `ATM_CARD.txt` file exists on the flash drive.
  - Creates the file if it does not exist.
- **readDataFromCard()**:
  - Reads account data from the `ATM_CARD.txt` file on the flash drive.
- **writeDataToCard()**:
  - Writes a new account's details or updates an existing account's information to the `ATM_CARD.txt` file on the flash drive.
- **rewriteAllAccountsToCard()**:
  - Rewrites all the account details to the `ATM_CARD.txt` file, ensuring that the file is updated after any transaction.

# Troubleshooting
- **Flash Drive Not Detected**:
  - Ensure the USB flash drive is correctly connected to the system.
  - Check if the drive is recognized by the operating system by opening "My Computer" or "This PC."
  - Run the program with administrative privileges if necessary.
- **File Access Issues**:
  - If the program cannot access or create the `ATM_CARD.txt` file, ensure that the flash drive has write permissions.
  - Try formatting the drive or using a different USB port.
  
