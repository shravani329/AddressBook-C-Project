#include <stdio.h>
#include "contact.h"
#include <stdio_ext.h>

int main() 
{
   
    AddressBook addressBook;
    addressBook.contactCount = 0;
    initialize(&addressBook);
     // Initialize the address book
     
     int choice = 0;

    do
    {
        printf( MAGENTA "\nAddress Book Menu:\n" RESET);
        printf( WHITE "1. Create contact\n" RESET);
        printf( WHITE "2. Search contact\n" RESET);
        printf( WHITE "3. Edit contact\n" RESET);
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save and Exit\n");
       
        printf(GREEN "Enter the choice: " RESET);
        scanf("%d", &choice);
        __fpurge(stdin);
        switch (choice)
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:
                listContacts(&addressBook);
                break;
            case 6:
                printf("Saving and Exiting...\n");
                saveContactsToFile(&addressBook);
                break;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    } while (choice != 6);
    
    return 0;
}
