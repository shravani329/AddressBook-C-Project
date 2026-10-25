#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria
     Contact temp;

    for(int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for(int j = 0; j < addressBook->contactCount - 1 - i; j++)
        {
            if(strcmp(addressBook->contacts[j].name,
                    addressBook->contacts[j + 1].name) > 0)
            {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }
    

    //Printing the Contactlist
    printf(BLUE "%-7s %-15s %-12s %-30s\n",
            "Sr.no", "Name", "Phone", "Email" RESET);
    for(int i=0; i < addressBook->contactCount ; i++)
    {

        printf("%-7d %-15s %-12s %-30s\n", i+1,
            addressBook->contacts[i].name,
            addressBook->contacts[i].phone,
            addressBook->contacts[i].email);

    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

int validate_name(char *name)
{
    int i;

    for(i = 0; name[i] != '\0'; i++)
    {
        if(!((name[i] >= 'A' && name[i] <= 'Z') ||
            (name[i] >= 'a' && name[i] <= 'z') ||
            name[i] == ' ' ||
            name[i] == '.'))
            {
                return 0;
            }
    }

    return 1;
}

int validate_phone(AddressBook *addressBook, char *phone)
{
    int i;

    for(i = 0; phone[i] != '\0'; i++)
    {
        if(phone[i] < '0' || phone[i] > '9' )
        {
            return 0;
        }
    }
    if(i != 10)
    {
        return 0;
    }

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            return 2;
        }
    }


    return 1;
}

int validate_email(AddressBook *addressBook,char *email)
{
    int i;
    int at = 0;
    int at_position = -1;
    int dot_position = -1;

    /* First character should not be a digit or @ */
    if((email[0] >= '0' && email[0] <= '9') || email[0] == '@')
    {
        return 0;
    }

    /* Check characters */
    for(i = 0; email[i] != '\0'; i++)
    {
        /* Uppercase letters are not allowed */
        if(email[i] >= 'A' && email[i] <= 'Z')
        {
            return 0;
        }

        /* @ should occur only once */
        if(email[i] == '@')
        {
            if(at == 1)
            {
                return 0;
            }

            at = 1;
            at_position = i;
        }
    }

    /* @ should be present */
    if(at == 0)
    {
        return 0;
    }

    /* Something should be present before @ */
    if(at_position == 0)
    {
        return 0;
    }

    /* .com should be at the end */
    if(strstr(email, ".com") == NULL)
    {
        return 0;
    }

    if(strcmp(email + strlen(email) - 4, ".com") != 0)
    {
        return 0;
    }

    /* Find position of .com */
    dot_position = strlen(email) - 4;

    /* Something should be present between @ and .com */
    if(dot_position <= at_position + 1)
    {
        return 0;
    }

    /* for unique email */
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].email, email) == 0)
        {
            return 2;
        }
    }

    return 1;
}

void createContact(AddressBook *addressBook)
{
    char name[50];
    char phone[20];
    char email[50];
    int res1,res2,res3;

    do{
        printf( CYAN"Enter the name : " RESET);
        scanf(" %[^\n]", name);

        res1 = validate_name(name);

        if(res1 == 0)
        {
            printf(RED "It's Invalid name.\n" RESET);
        }
    }while(res1 == 0);

    do
    {
        printf(CYAN "Enter the phone number: " RESET);
        scanf("%s", phone);

        res2 = validate_phone(addressBook,phone);

        if(res2 == 0)
        {
            printf( RED "It's Invalid phone number\n" RESET);
            continue;
        }

        else if(res2 == 2)
        {
            printf( YELLOW "Phone number already exists\n" RESET);
        }

    } while(res2 != 1);

    do
    {
        printf(CYAN "Enter the email: " RESET);
        scanf("%s",email);

        res3 = validate_email(addressBook,email);

        if(res3 == 0)
        {
            printf(RED "It's Invalid email.\n" RESET);
        }
        else if(res3 == 2)
        {
            printf(YELLOW "Email already exists\n" RESET);
        }
    }while(res3 != 1);


    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
    strcpy(addressBook->contacts[addressBook->contactCount].email,email);
    addressBook->contactCount++;
    printf(YELLOW "Contact created successfully.\n" RESET);
}

int search_name(AddressBook *addressBook, char *name)
{
    int i;
    int count = 0;

    for(i = 0; i < addressBook->contactCount; i++)
    {
       
        if(strcmp(addressBook->contacts[i].name, name) == 0)
        {
            printf("%d\t%s\t%s\t%s\n",
                    i + 1,
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);

            count++;
        }
    }

    if(count == 0)
    {
        return -1;
    }

    return count;
}

int search_phone(AddressBook *addressBook, char *phone)
{
    int i;

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            return i;
            break;
        }
    }

    return -1;
}

int search_email(AddressBook *addressBook, char *email)
{
    int i;

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].email, email) == 0)
        {
            return i;
        }
    }

    return -1;
}

void searchContact(AddressBook *addressBook)
{
    /* Define the logic for search */
    int choice;
    int index;

    char name[50];
    char phone[20];
    char email[50];

    printf(MAGENTA "Search Contact\n" RESET);
    printf(WHITE "1. Name\n" RESET);
    printf(WHITE "2. Phone\n"RESET);
    printf(WHITE "3. Email\n"RESET);
    printf(WHITE "4. Exit\n"RESET);

    printf(GREEN "Enter the choice: " RESET);
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            {
                int count;
                printf( BLUE "Enter the name: " RESET);
                scanf("%s",name);

                count = search_name(addressBook,name);
                if(count == -1)
                {
                    printf(RED "Name not found\n" RESET);
                }
                else if(count == 1)
                {
                    for(int i = 0; i < addressBook->contactCount; i++)
                    {
                        if(strcmp(addressBook->contacts[i].name, name) == 0)
                        {
                            index = i;
                            break;
                        }
                    }
                }
                else if(count > 1)
               {
                    int serial;
                    printf(BLUE "Enter the serial number: " RESET);
                    scanf("%d", &serial);
                    index = serial - 1;

                    if(index >= 0 && index < addressBook->contactCount &&
                    strcmp(addressBook->contacts[index].name, name) == 0)
                    {
                        printf("%d\t%s\t%s\t%s\n",
                                index + 1,
                                addressBook->contacts[index].name,
                                addressBook->contacts[index].phone,
                                addressBook->contacts[index].email);
                    }
                    else
                    {
                        printf(RED "Invalid serial number\n" RESET);
                    }
                }
                break;
            }

        case 2:
            {
                    printf( BLUE "Enter the phone number: " RESET);
                    scanf("%s",phone);

                    index = search_phone(addressBook,phone);
                    if(index == -1)
                    {
                        printf(RED "Phone number not found\n" RESET);
                    }
                    else
                    {
                        printf(YELLOW "Contact found\n" RESET);
                        printf("%d\t%s\t%s\t%s\n",
                                    index + 1,
                                    addressBook->contacts[index].name,
                                    addressBook->contacts[index].phone,
                                    addressBook->contacts[index].email);
                    }
               
                break;
            }

        case 3:
            {
                    printf(BLUE "Enter the email: " RESET);
                    scanf("%s",email);

                    index = search_email(addressBook,email);
                    if(index == -1)
                    {
                        printf(RED "Email not found\n" RESET);
                    }
                    else
                    {
                        printf(YELLOW "Contact  Found\n" RESET);
                        printf("%d\t%s\t%s\t%s\n",
                                    index + 1,
                                    addressBook->contacts[index].name,
                                    addressBook->contacts[index].phone,
                                    addressBook->contacts[index].email );
                    }
                break;
            }

        case 4:
            return;

        default:
            printf(RED "Invalid choice\n" RESET);
    }
}

void editContact(AddressBook *addressBook)
{
    int choice;
    int index = -1;
    int count;
    int serial;

    char name[50];
    char phone[20];
    char email[50];

    printf(MAGENTA"\nEdit Contact\n" RESET);
    printf(WHITE "1.Search by Name\n" RESET);
    printf(WHITE "2.Search by Phone\n" RESET);
    printf(WHITE "3.Search by Email\n" RESET);
    printf(WHITE "4. Exit\n" RESET);

    printf(GREEN "Enter the choice: " RESET);
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
        {
            do
            {
                printf(BLUE "Enter the name: " RESET);
                scanf("%s", name);

                count = search_name(addressBook, name);

                if(count == -1)
                {
                    printf(RED "Name not found\n" RESET);
                }
                else if(count == 1)
                {
                    for(int i = 0; i < addressBook->contactCount; i++)
                    {
                        if(strcmp(addressBook->contacts[i].name, name) == 0)
                        {
                            index = i;
                            break;
                        }
                    }
                }
                else
                {
                   /* Multiple contacts found */

                    do
                    {
                        printf(BLUE "Enter the serial number: " RESET);
                        scanf("%d", &serial);

                        index = serial - 1;

                        if(index < 0 || index >= addressBook->contactCount ||
                        strcmp(addressBook->contacts[index].name, name) != 0)
                        {
                          printf(RED "Invalid serial number\n" RESET);
                          index = -1;
                        }
                    }while(index == -1);
                }
            } while(count == -1);
            break;
        }


        case 2:
        {

            do
            {
                printf(BLUE "Enter the phone number: " RESET);
                scanf("%s", phone);

                index = search_phone(addressBook, phone);

                if(index == -1)
                {
                    printf(RED "Phone number not found\n" RESET);
                    return;
                }
               else
                {
                    printf("%d\t%s\t%s\t%s\n",
                        index + 1,
                        addressBook->contacts[index].name,
                        addressBook->contacts[index].phone,
                        addressBook->contacts[index].email);
                }
            }while(index == -1);
            break;
        }

        case 3:
        {
            do
            {
                printf(BLUE "Enter the email: " RESET);
                scanf("%s", email);

                index = search_email(addressBook, email);

                if(index == -1)
                {
                    printf(RED "Email not found\n" RESET);
                }
                else
                {
                    printf("%d\t%s\t%s\t%s\n",
                        index + 1,
                        addressBook->contacts[index].name,
                        addressBook->contacts[index].phone,
                        addressBook->contacts[index].email);
                }
               
            }while(index == -1);
            break;
        }

        case 4:
            return;

        default:
            printf(RED "Invalid choice\n" RESET);
            return;

    }


    if(index != -1)
        {

            int field;

            printf(YELLOW "Contact Found:\n" RESET);
            printf(GREEN "Enter the field to edit:\n" RESET);
            printf(WHITE"1. Name\n" RESET);
            printf(WHITE"2. Phone\n" RESET);
            printf(WHITE"3. Email\n" RESET);

            printf(CYAN "Enter the choice: " RESET);
            scanf("%d", &field);

            switch(field)
            {
                case 1:
                {
                    char new_name[50];
                    int validname;

                    do
                    {
                        printf(CYAN "Enter new name: " RESET);
                        scanf(" %[^\n]", new_name);

                        validname = validate_name(new_name);
                        if(validname == 0)
                        {
                            printf(RED "Invalid name! Enter again\n" RESET);
                        }
                    }while(validname == 0);

                    strcpy(addressBook->contacts[index].name, new_name);
                    printf( YELLOW "\n Contact Updated Successfully \n" RESET);
                    printf("%d\t%s\t%s\t%s\n",
                            index + 1,
                            addressBook->contacts[index].name,
                            addressBook->contacts[index].phone,
                            addressBook->contacts[index].email);

                    break;
                }

                case 2:
                {
                    char new_phone[20];
                    int valid;
                    do
                    {
                        printf(CYAN "Enter new phone number: " RESET);
                        scanf("%s", new_phone);

                        valid = validate_phone(addressBook, new_phone);
                        
                        if(valid == 2 &&
                            strcmp(addressBook->contacts[index].phone, phone) == 0)
                        {
                            valid = 1;
                        }

                        if(valid  == 0)
                        {
                            printf(RED "Invalid phone number! Enter again\n" RESET);
                        }
                        else if(valid == 2)
                        {
                            printf(YELLOW "Phone number already exists! Enter again\n" RESET);
                        }
                    } while(valid != 1);

                    strcpy(addressBook->contacts[index].phone, new_phone);
                    printf(YELLOW "\n Contact Updated Successfully \n" RESET);
                    printf("%d\t%s\t%s\t%s\n",
                                index + 1,
                                addressBook->contacts[index].name,
                                addressBook->contacts[index].phone,
                                addressBook->contacts[index].email);

                    break;
                }

                case 3:
                {
                    char new_email[50];
                    int validmail;

                    do
                    {
                        printf(CYAN "Enter new email: " RESET);
                        scanf("%s", new_email);

                        validmail =  validate_email(addressBook,new_email);
                        
                        if(validmail == 2 &&
                            strcmp(addressBook->contacts[index].email, email) == 0)
                        {
                           validmail = 1;
                        }
                        if(validmail == 0)
                        {
                            printf(RED "Invalid email! Enter again\n" RESET);
                        }
                        else if(validmail == 2)
                        {
                            printf(YELLOW "Email already exists! Enter again\n"RESET);
                        }
                    } while(validmail != 1);

                    strcpy(addressBook->contacts[index].email, new_email);
                    printf(YELLOW "\n Contact Updated Email: \n" RESET);
                    printf("%d\t%s\t%s\t%s\n",
                                index + 1,
                                addressBook->contacts[index].name,
                                addressBook->contacts[index].phone,
                                addressBook->contacts[index].email);
                    break;
                }
                default:
                printf(RED "Invalid field choice\n" RESET);
                break;
            }
        }
    }
 
void deleteContact(AddressBook *addressBook)
{
    int choice;
    int index = -1;
    int i;

    printf(MAGENTA "Delete Contact\n" RESET);
    printf(CYAN "Search by:\n" RESET);
    printf(WHITE "1. Name\n" RESET);
    printf(WHITE "2. Phone\n" RESET);
    printf(WHITE "3. Email\n" RESET);
    printf(WHITE "4. Exit\n" RESET);
    printf(GREEN "Enter the choice: " RESET);
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
        {
            char name[50];
            int count;
            int serial;

            do
            {
                printf(CYAN "Enter the name: " RESET);
                scanf("%s", name);

                count = search_name(addressBook, name);

                if(count == -1)
                {
                    printf(RED "Name not found\n" RESET);
                }
                else if(count == 1)
                {
                    for(int i = 0; i < addressBook->contactCount; i++)
                    {
                        if(strcmp(addressBook->contacts[i].name, name) == 0)
                        {
                            index = i;
                            break;
                        }
                    }
                }
                else
                {
                    do
                    {
                        printf(CYAN "Enter the serial number: " RESET);
                        scanf("%d", &serial);

                        index = serial - 1;
                        if(index < 0 || index >= addressBook->contactCount ||
                            strcmp(addressBook->contacts[index].name, name) != 0)
                        {
                            printf(RED "Invalid serial number\n" RESET);
                            index = -1;
                        }
                    } while(index == -1);
                }
            } while(count == -1);
            break;
        }

        case 2:
        {
            char phone[20];

            do
            {
                printf(CYAN "Enter the phone number: " RESET);
                scanf("%s", phone);

                index = search_phone(addressBook, phone);

                if(index == -1)
                {
                    printf(RED "Phone number not found\n" RESET);
                }

            } while(index == -1);

            break;
        }

        case 3:
        {
            char email[50];

            do
            {
                printf(CYAN "Enter the email: " RESET);
                scanf("%s", email);

                index = search_email(addressBook, email);

                if(index == -1)
                {
                    printf(RED "Email not found\n" RESET);
                }

            } while(index == -1);

            break;
        }

        case 4:
            return;

        default:
            printf(RED "Invalid choice\n" RESET );
            return;
    }
    /* Delete the contact */

    for(i = index; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    addressBook->contactCount--;

    printf(YELLOW "Contact deleted successfully\n" RESET);
}