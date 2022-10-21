#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_ROOMS 100
//daily room rate : 150

typedef struct{

    char *firstName;
    char *lastName;
}customerInformation;

typedef struct{

int day;
int month;
int year;
}dateInformation;

typedef struct{

int numberOfPeople;
int isEmpty;
int roomNumber;
double priceInformation;
customerInformation customer;
dateInformation dateOfEntry;
dateInformation releaseDate;
}roomInformation;

typedef enum{

    customer = 0, employee = 1
}authorityLevel;

int dayQuery(dateInformation dateOfEntry, dateInformation releaseDate){
	if(dateOfEntry.day > releaseDate.day){ // We are checking to ensure that there are no minuses.
		releaseDate.month -= 1;
		releaseDate.day += 30;
	}
		if(dateOfEntry.month> releaseDate.month){
		releaseDate.year -= 1;
		releaseDate.month += 12;
	}
	dateInformation dateDifference;
	dateDifference.day = releaseDate.day - dateOfEntry.day;
	dateDifference.month = releaseDate.month  - dateOfEntry.month;
	dateDifference.year = releaseDate.year - dateOfEntry.year;

	int dayDifference = dateDifference.year * 365 + dateDifference.month * 30 + dateDifference.day;
	return dayDifference;
}

double priceInquiry(roomInformation *rooms, int roomNumber){

	int dayDifference = dayQuery(rooms[roomNumber - 1].dateOfEntry, rooms[roomNumber - 1].releaseDate);
	printf("The number of days you will stay:%d\n", dayDifference);
	return dayDifference * 150 * rooms[roomNumber - 1].numberOfPeople;
}

void clearReservation(roomInformation *rooms, int roomNumber){

	rooms[roomNumber  - 1].isEmpty = 0;
	free(rooms[roomNumber - 1].customer.firstName);
	free(rooms[roomNumber - 1].customer.lastName);

	printf("Check-out / cancellation of room %d completed.\n", roomNumber);
}

void fullRoomInquiry(roomInformation *rooms){

	int i;
	for(i = 0; i < NUMBER_OF_ROOMS; i++){
		if(rooms[i].isEmpty != 0){
			printf("---------------------------------------\n");
			printf("Room number:%d\n", rooms[i].roomNumber);
			printf("Customer name - surname:%s %s\n", rooms[i].customer.firstName, rooms[i].customer.lastName);
			printf("Number of people:%d\n", rooms[i].numberOfPeople);
			printf("Price information:%d\n", rooms[i].priceInformation);
			printf("Date of entry:%d.%d.%d\n",rooms[i].dateOfEntry.day, rooms[i].dateOfEntry.month, rooms[i].dateOfEntry.year);
			printf("Release date:%d.%d.%d\n",rooms[i].releaseDate.day, rooms[i].releaseDate.month, rooms[i].releaseDate.year);
		}
	}
}

void addNewCustomer(roomInformation *rooms){

	int roomInquiry;
	char firstName[20], lastName[20], ch;

	printf("Please choose your room:\n");
	scanf("%d", &roomInquiry);

	if(rooms[roomInquiry-1].isEmpty != 0){
		printf("The room you want to select is occupied until %d.%d.%d.\n", rooms[roomInquiry-1].releaseDate.day, rooms[roomInquiry-1].releaseDate.month, rooms[roomInquiry-1].releaseDate.year);
		return;
	}
	rooms[roomInquiry-1].roomNumber = roomInquiry;

	printf("Please enter your first and last name:\n");
	scanf("%s%s",&firstName, &lastName);

	rooms[roomInquiry-1].customer.firstName = (char*)malloc(sizeof(char)*strlen(firstName) + 1); //we add +1 for null in the last part.
	rooms[roomInquiry-1].customer.lastName = (char*)malloc(sizeof(char)*strlen(lastName) + 1);

	strcpy(rooms[roomInquiry - 1].customer.firstName, firstName);
	strcpy(rooms[roomInquiry - 1].customer.lastName, lastName);

	printf("Please enter the number of people:\n");
	scanf("%d", &rooms[roomInquiry-1].numberOfPeople);

	printf("Please enter the date of arrival at the hotel in the form of day month year:\n");
	scanf("%d.%d.%d", &rooms[roomInquiry-1].dateOfEntry.day, &rooms[roomInquiry-1].dateOfEntry.month, &rooms[roomInquiry-1].dateOfEntry.year);

	printf("Please enter the date you will leave the hotel as day month year:\n");
	scanf("%d.%d.%d", &rooms[roomInquiry-1].releaseDate.day, &rooms[roomInquiry-1].releaseDate.month, &rooms[roomInquiry-1].releaseDate.year);

	rooms[roomInquiry - 1].priceInformation = priceInquiry(rooms, roomInquiry);

	printf("Total price you have to pay: %.2lf Tl\n", rooms[roomInquiry - 1].priceInformation);
	printf("Do you confirm(Y-N): ");
	fflush(stdin);
	ch = getchar();

	if(ch == 'H'){
		clearReservation(rooms, roomInquiry);
		return;
	}
	else{
		rooms[roomInquiry - 1].isEmpty = 1;
	}
}

void roomInformationInquery(roomInformation *rooms, int roomNumber){
	if(rooms[roomNumber- 1].isEmpty != 0){
		printf("Room %d is occupied until %d.%d.%d\n", roomNumber, rooms[roomNumber - 1].releaseDate.day, rooms[roomNumber - 1].releaseDate.month, rooms[roomNumber - 1].releaseDate.year);
	}
	else{
		printf("Room %d is currently vacant.", roomNumber);
	}
}

void priceInformationInquiry(){

	int numberOfPeople;
	double totalPrice = 0.0;
	dateInformation dateOfEntry, releaseDate;

	printf("Please enter the number of people:\n");
	scanf("%d", &numberOfPeople);

	printf("Please enter the date of arrival at the hotel in the form of day month year:\n");
	scanf("%d.%d.%d", &dateOfEntry.day, &dateOfEntry.month, &dateOfEntry.year);

	printf("Please enter the date you will leave the hotel as day month year:\n");
	scanf("%d.%d.%d", &releaseDate.day, &releaseDate.month, &releaseDate.year);

	totalPrice = dayQuery(dateOfEntry, releaseDate) * numberOfPeople * 150;
	printf("Total amount you have to pay: %.2lf Tl\n", totalPrice);
}

void backupRecord(roomInformation *rooms){
	int i;
	FILE *hotelRecords = fopen("hotelRecords.txt", "a");
	if(hotelRecords == NULL){
		fprintf(stderr, "An error occurred while retrieving the backup record.");
		exit(1);
	}
	for(i = 0; i < NUMBER_OF_ROOMS; i++){
		if(rooms[i].isEmpty != 0){
			fprintf(hotelRecords, "-------------------------------------------\n");
			fprintf(hotelRecords, "Room number:%d\n", rooms[i].roomNumber);
			fprintf(hotelRecords, "Customer name - surname: %s %s\n", rooms[i].customer.firstName, rooms[i].customer.lastName);
			fprintf(hotelRecords, "Number of people:%d\n", rooms[i].numberOfPeople);
			fprintf(hotelRecords, "Total price:%.2lf Tl\n", rooms[i].priceInformation);
			fprintf(hotelRecords, "Date of entry:%d.%d.%d\n", rooms[i].dateOfEntry.day, rooms[i].dateOfEntry.month, rooms[i].dateOfEntry.year);
			fprintf(hotelRecords, "Release date:%d.%d.%d\n", rooms[i].releaseDate.day, rooms[i].releaseDate.month, rooms[i].releaseDate.year);
		}
	}
	fclose(hotelRecords);
}

int main()
{

    int fixedPassword = 123456, userPassword, roomNumber;
    char ch;

    authorityLevel userAuthority;

    roomInformation *room;
    room = (roomInformation*)calloc(NUMBER_OF_ROOMS,sizeof(roomInformation));

    printf("\t\t\t Welcome!\n");

    printf("Please enter your employee password:");
    scanf("%d", &userPassword);

    if(fixedPassword == userPassword){
        userAuthority = employee;
    }
    else{
        userAuthority = customer;
    }

    do{
            printf("New customer login:A\ninquire room information: B\nfull room information: C\nprice inquiry: D\ncustomer exit: E\nexit: Q\n");

    fflush(stdin);
    ch = getchar();

    switch(ch){
        case 'A':
        case 'a':
        	if(userAuthority == employee){
        		addNewCustomer(room);
			}
			else{

				printf("This field is for working personnel only.");
			}
        break;
        case 'B':
        case 'b':
        	printf("Please enter the number of the room you want to query:\n");
        	scanf("%d", &roomNumber);
        	roomInformationInquery(room, roomNumber);
        break;
        case 'C':
        case 'c':
		    if(userAuthority == employee){
		    	fullRoomInquiry(room);
			}
			else{

			printf("This field is for working personnel only.");
			}
        break;
        case 'D':
        case 'd':
        	priceInformationInquiry();
        break;
        case 'E':
        case 'e':
        	if(userAuthority == employee){
        		printf("Please enter the number of the room you want to check out:\n");
        		scanf("%d", &roomNumber);
		    	clearReservation(room, roomNumber);
			}
			else{

			printf("This field is for working personnel only.");
			}
        break;
        case 'q':
        	backupRecord(room);
        break;
    default:
    	printf("Please enter a valid letter.\n");
        break;
    }

}while(ch != 'q');


    return 0;
}

