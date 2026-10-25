#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
   // Save contacts to file

    FILE *fp = fopen("contacts.txt","w");
    
    if(fp == NULL)
    {
        return;
    }
    fprintf(fp,"#%d\n",addressBook->contactCount);
    for(int i=0;i < addressBook->contactCount;i++)
    {
         fprintf(fp, "%s,%s,%s\n",
            addressBook->contacts[i].name,
            addressBook->contacts[i].phone,
        addressBook->contacts[i].email);
    }
    fclose(fp);
    printf(YELLOW "Contact Saved Successfully !" RESET);
}

void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contacts.txt", "r");

    if(fp == NULL)
    {
        return;
    }

    fscanf(fp, "#%d\n", &addressBook->contactCount);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(fp, "%49[^,],%[^,],%[^\n]\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    fclose(fp);
}
