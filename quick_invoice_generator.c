#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <string.h> 

// Struct for items
struct items {
    char item[20];
    float price;
    int qty;
};

// Struct for orders
struct orders {
    char customer[50];
    char date[50];
    int invoiceID;
    int numOfItems;
    struct items *itm;  // Dynamic array for items
    float discountRate; // Discount percentage (e.g., 0.10 for 10%)
    float taxRate;      // Tax percentage (e.g., 0.13 for 13%)
};

// Function to generate a unique invoice ID
int generateInvoiceID() {
    srand(time(0)); // Seed for random number generation
    return rand();  // Generate random invoice ID
}

// Function to generate bill header
void generateBillHeader(char name[50], char date[30], int invoiceID) {
    printf("\n\n");
    printf("\t    SY. Restaurant (Prices in CAD)");
    printf("\n\t   -------------------------------");
    printf("\nDate: %s", date);
    printf("\nInvoice ID: %d", invoiceID);  // Display invoice ID
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\tQty\t\tTotal\n");
    printf("---------------------------------------\n");
}

// Function to generate bill body (items)
void generateBillBody(const char *item, int qty, float price) {
    printf("%-20s %10d %15.2f\n", item, qty, qty * price);
}

// Function to generate bill footer
void generateBillFooter(float total, float discountRate, float taxRate) {
    printf("\n---------------------------------------");
    float discount = total * discountRate;
    float netTotal = total - discount;
    float tax = netTotal * taxRate;
    float grandTotal = netTotal + tax;

    printf("\nSub Total\t\t\t%.2f", total);
    printf("\nDiscount @%.2f%%\t\t\t-%.2f", discountRate * 100, discount);
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nTax @%.2f%%\t\t\t+%.2f", taxRate * 100, tax);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}

// Function to save the invoice to a file
void saveInvoiceToFile(struct orders ord, float total) {
    char filename[50];
    sprintf(filename, "Invoice_%d.txt", ord.invoiceID);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error saving the invoice!\n");
        return;
    }

    fprintf(fp, "Customer: %s\n", ord.customer);
    fprintf(fp, "Date: %s\n", ord.date);
    fprintf(fp, "Invoice ID: %d\n", ord.invoiceID);
    fprintf(fp, "Discount Rate: %.2f%%\n", ord.discountRate * 100);
    fprintf(fp, "Tax Rate: %.2f%%\n", ord.taxRate * 100);
    fprintf(fp, "---------------------------------------\n");
    fprintf(fp, "Items\t\tQty\t\tTotal\n");
    fprintf(fp, "---------------------------------------\n");

    float discount = total * ord.discountRate;
    float netTotal = total - discount;
    float tax = netTotal * ord.taxRate;
    float grandTotal = netTotal + tax;

    for (int i = 0; i < ord.numOfItems; i++) {
        fprintf(fp, "%-20s %10d %15.2f\n", ord.itm[i].item, ord.itm[i].qty, ord.itm[i].qty * ord.itm[i].price);
    }

    fprintf(fp, "---------------------------------------\n");
    fprintf(fp, "Sub Total\t\t\t%.2f\n", total);
    fprintf(fp, "Discount @%.2f%%\t\t\t-%.2f\n", ord.discountRate * 100, discount);
    fprintf(fp, "Net Total\t\t\t%.2f\n", netTotal);
    fprintf(fp, "Tax @%.2f%%\t\t\t+%.2f\n", ord.taxRate * 100, tax);
    fprintf(fp, "---------------------------------------\n");
    fprintf(fp, "Grand Total\t\t\t%.2f\n", grandTotal);
    fprintf(fp, "---------------------------------------\n");

    fclose(fp);
    printf("Invoice saved as %s\n", filename);
}

// Function to load and display an existing invoice
void loadInvoiceFromFile(int invoiceID) {
    char filename[50];
    sprintf(filename, "Invoice_%d.txt", invoiceID);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Invoice not found!\n");
        return;
    }

    char line[100];
    printf("\n***** Invoice Details *****\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

// Function to convert a string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to trim the newline character from a string, if present
void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Modified function for case-insensitive search
void searchInvoiceByCustomer(const char *customerName) {
    char filename[50];
    int found = 0;
    char lowerCustomerName[50];
    strcpy(lowerCustomerName, customerName);
    toLowerCase(lowerCustomerName); // Convert the search query to lowercase

    for (int i = 1; i <= 100; i++) { // Assuming a maximum of 100 invoices for simplicity
        sprintf(filename, "Invoice_%d.txt", i);
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            continue; // Skip if file does not exist
        }

        char line[200]; // Increased buffer size to 200 for longer lines
        while (fgets(line, sizeof(line), fp)) {
            trimNewline(line); // Remove trailing newline from the line
            char lowerLine[200];
            strcpy(lowerLine, line);
            toLowerCase(lowerLine); // Convert each line to lowercase before comparing

            if (strstr(lowerLine, lowerCustomerName) != NULL) {
                printf("Invoice found: %s\n", filename);
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found) break;
    }

    if (!found) {
        printf("No invoices found for customer: %s\n", customerName);
    }
}

// Input validation functions
int getValidatedPositiveInt(const char *prompt) {
    int val;
    do {
        printf("%s", prompt);
        scanf("%d", &val);
        if (val <= 0) {
            printf("Invalid input! Value must be greater than 0.\n");
        }
    } while (val <= 0);
    return val;
}

float getValidatedPositiveFloat(const char *prompt) {
    float val;
    do {
        printf("%s", prompt);
        scanf("%f", &val);
        if (val <= 0) {
            printf("Invalid input! Value must be greater than 0.\n");
        }
    } while (val <= 0);
    return val;
}

void getValidatedString(const char *prompt, char *str, int length) {
    do {
        printf("%s", prompt);
        fgets(str, length, stdin);
        str[strlen(str) - 1] = '\0'; // Remove trailing newline
        if (strlen(str) == 0) {
            printf("Input cannot be empty! Please enter a valid string.\n");
        }
    } while (strlen(str) == 0);
}

int main() {
    struct orders ord;
    int choice;
    float total = 0;

    while (1) {
        printf("\n1. Generate Invoice");
        printf("\n2. Load Invoice");
        printf("\n3. Search Invoice by Customer");
        printf("\n4. Exit");
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        fgetc(stdin); // Clear the input buffer

        switch (choice) {
            case 1:
                // Generate Invoice
                getValidatedString("Enter customer name: ", ord.customer, 50);
                strcpy(ord.date, __DATE__);
                ord.invoiceID = generateInvoiceID();
                ord.numOfItems = getValidatedPositiveInt("Enter the number of items: ");
                ord.discountRate = getValidatedPositiveFloat("Enter discount percentage (e.g., 10 for 10%): ") / 100.0;
                ord.taxRate = getValidatedPositiveFloat("Enter tax percentage (e.g., 15 for 15%): ") / 100.0;

                // Dynamically allocate memory for items
                ord.itm = (struct items *)malloc(ord.numOfItems * sizeof(struct items));
                if (ord.itm == NULL) {
                    printf("Memory allocation failed!\n");
                    break;
                }

                for (int i = 0; i < ord.numOfItems; i++) {
                    getValidatedString("Enter item name: ", ord.itm[i].item, 20);
                    ord.itm[i].qty = getValidatedPositiveInt("Enter quantity: ");
                    ord.itm[i].price = getValidatedPositiveFloat("Enter unit price: ");
                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                // Display and save the invoice
                generateBillHeader(ord.customer, ord.date, ord.invoiceID);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }
                generateBillFooter(total, ord.discountRate, ord.taxRate);
                saveInvoiceToFile(ord, total);

                // Free dynamically allocated memory
                free(ord.itm);
                break;

            case 2:
                // Load an existing invoice
                printf("Enter Invoice ID to load: ");
                int invoiceID;
                scanf("%d", &invoiceID);
                loadInvoiceFromFile(invoiceID);
                break;

            case 3:
                // Search invoice by customer name
                char customerName[50];
                getValidatedString("Enter customer name to search: ", customerName, 50);
                searchInvoiceByCustomer(customerName);
                break;

            case 4:
                printf("Exiting the program.\n");
                exit(0);
                break;

            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }
    }

    return 0;
}
