// Barry Chen, Faculty of Arts, Department of Computer Science, McGill ID: 260952566
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Record / Node for the linked list.
// You MUST use the exact structure. No modification allowed.
typedef struct PersonalInfoRecord
{ 
	char id[10];
	char ptype;
	char name[31];

	union
	{
		struct
		{
			char dept[31];
			int hireyear;
			char tenured;
		}prof;
		struct
		{
			char faculty[31];
			int admyear;
		}stud;
	}info;

	struct PersonalInfoRecord *next;
} PersonalInfoRecord;

struct PersonalInfoRecord *getNewNode(char *record[]){ // create a new node
	PersonalInfoRecord *pir = malloc(sizeof(struct PersonalInfoRecord));
	pir->next = NULL;
	if (strcmp(record[2],"S") == 0){ // create student record
		strcpy(pir->id, record[1]);
		pir->ptype = *(record[2]);
		strcpy(pir->name, record[3]);
		strcpy(pir->info.stud.faculty, record[4]);
		pir->info.stud.admyear = atoi(record[5]);
	}
	if (strcmp(record[2],"P") == 0){ // create professor record
		strcpy(pir->id, record[1]);
		pir->ptype = *(record[2]);
		strcpy(pir->name, record[3]);
		strcpy(pir->info.prof.dept, record[4]);
		pir->info.prof.hireyear = atoi(record[5]);
		pir->info.prof.tenured = *(record[6]);
	}
	return pir;
}

void updateRecord(struct PersonalInfoRecord *src, struct PersonalInfoRecord *dest){ // update the record of a single existed node
	switch(src->ptype){
		case 'S':   //update the record of professor  
			if (strcmp(src->name, "") != 0){
				strcpy(dest->name, src->name);
			}
			if (strcmp(src->info.stud.faculty, "") != 0){
				strcpy(dest->info.stud.faculty, src->info.stud.faculty);
			}
			if (src->info.stud.admyear != 0){
				dest->info.stud.admyear = src->info.stud.admyear;
			}
			break;
		case 'P':   //update the record of student
			if (strcmp(src->name, "") != 0){
				strcpy(dest->name, src->name);
			}
			if (strcmp(src->info.prof.dept, "") != 0){
				strcpy(dest->info.prof.dept, src->info.prof.dept);
			}
			if (src->info.prof.hireyear != 0){
				dest->info.prof.hireyear = src->info.prof.hireyear;
			}
			if (src->info.prof.tenured != '\0'){
				dest->info.prof.tenured = src->info.prof.tenured;
			}
			break;
	}
}

struct PersonalInfoRecord *updateList(struct PersonalInfoRecord *first, char *record[]){
	
	int flag = 0; // record whether the new node is existed or not
	int flag1 = 0; // mark if the the node that is going to be deleted is existed or not
	struct PersonalInfoRecord *added = NULL; //added pointer points to either added node or deleted node

	if (strcmp(record[0], "D") == 0 ){ //delete the node
			if(first == NULL){ 
				return NULL;
			}
			if (strcmp(first->id, record[1]) == 0){ // delete the head node
				return first;
			}else{  // delete node that is not the head node
				struct PersonalInfoRecord *t = first;
				while(t->next != NULL){   //better to implement binary search? too lazy to try... 
					if(strcmp(t->next->id, record[1]) == 0){
						flag1 = 1;
						struct PersonalInfoRecord *deleted = t->next;
						added = deleted;
						t->next = t->next->next;
						return added;
					} 
					t = t->next;
				}
				if (flag1 == 0){ // delete a non-existed node 
					return NULL;
				}
				return added;
			}
	}else{ // insert or update
		struct PersonalInfoRecord *newpir = getNewNode(record);
		if (first != NULL){  //update the node
			struct PersonalInfoRecord *temp = NULL;
			temp = first;
			while (temp != NULL){
				if (strcmp(temp->id, newpir->id) == 0){
					flag = 1;
					updateRecord(newpir, temp);
					return NULL;
				}
				temp = temp->next;
			}
		}
		if (flag == 0){ //insert the node
			added = newpir;
			if(first == NULL ){ //insert when the list is empty
				return added;
			}else{
				int newid = atoi(newpir->id);
				if(first->next == NULL){ //insert at the front or after the head node when there's only head node
					int firstid = atoi(first->id);
					if (newid < firstid){
						newpir->next = first;
					}else{
						first->next = newpir;
					}
				}else{ 
					struct PersonalInfoRecord *tmp = first;
					if (atoi(newpir->id) < atoi(tmp->id)){ //insert at the front of the head node
						newpir->next = tmp;
						return added;
					}
					while (atoi(tmp->next->id) < newid){ 
						tmp = tmp->next;
						if(tmp->next == NULL){ //insert at end
							tmp->next = newpir;
							return added;
						}
					}
					newpir->next = tmp->next;  //insert at the middle
					tmp->next = newpir;
				}
			}
		}		
	}
	return added;
}

void printList(struct PersonalInfoRecord *first){ // traverse the list and print the information of each node to the std output
	if (first == NULL){
		printf("empty list\n");
	}
	struct PersonalInfoRecord *tmp = first;
	while (tmp != NULL){
		switch(tmp->ptype){
			case'S':
				printf("%s,%c,%s,%s,%d\n", tmp->id,tmp->ptype,tmp->name,tmp->info.stud.faculty,tmp->info.stud.admyear);
				break;
			case 'P':
				printf("%s,%c,%s,%s,%d,%c\n", tmp->id,tmp->ptype,tmp->name,tmp->info.prof.dept,tmp->info.prof.hireyear,tmp->info.prof.tenured);
				break;
		}
		tmp = tmp->next;
	}
}

void printToFile(FILE *file, struct PersonalInfoRecord *first){ // print the information of each node to the output file
	char empty[1] = {'\0'};
	if (first == NULL){  //empty list, printing nothing
		fprintf(file,"%s", empty);
	}
	struct PersonalInfoRecord *tmp = first;
	while (tmp != NULL){
		switch(tmp->ptype){
			case'S':
				fprintf(file,"%s,%c,%s,%s,%d\n", tmp->id,tmp->ptype,tmp->name,tmp->info.stud.faculty,tmp->info.stud.admyear);
				break;
			case 'P':
				fprintf(file,"%s,%c,%s,%s,%d,%c\n", tmp->id,tmp->ptype,tmp->name,tmp->info.prof.dept,tmp->info.prof.hireyear,tmp->info.prof.tenured);
				break;
		}
		tmp = tmp->next;
	}
}

void cleanMemory(struct PersonalInfoRecord *first){ //clean the memory allocated for all the nodes except the head.
	struct PersonalInfoRecord *tmp = first;
	while(tmp->next != NULL){
		tmp = tmp->next;
		free(tmp);
	}
}

int main(int argc, char *argv[]){
	
	if (argc != 2){ // check the input argument
		fprintf(stderr,"Error, please pass the database filename as the argument.\nUsage ./pimapp <dbfile>\n");
		exit(1);
	}

	char inputbuffer[100], *input; // to store each input line;
	char *info[100]; // to store each field date into array
	PersonalInfoRecord *head = NULL;
	PersonalInfoRecord *added = NULL; //added pointer points to either new added node or deleted node
	
	while (fgets(input=inputbuffer, 100, stdin) != NULL) // Get each input line.
	{
		
		if(strncmp(input, "END", 3) == 0){ 
			FILE *output = fopen(argv[1],"w");
			if (output == NULL){
				fprintf(stderr, "Error, unable to open %s for writing.\n", argv[1]);		
				exit(3);
			}else{
				printToFile(output,head);
			}
			if (head != NULL){ // free the memory alloced for all the nodes 
				cleanMemory(head);
				head = NULL;
				added = NULL;
			} 
			break;
		}
		
		int i = 0; char*fielddata;
		while( (fielddata = strsep(&input, ",\n")) != NULL){ //extract each field and store them into the info array
			info[i++] = fielddata;
			//printf("Field %d is %s\n", i, info[i-1]);
		}
	
		if (i < 7 && strcmp(info[0], "I") == 0){
			puts("wrong number of input field");
		}

		if (strcmp(info[0], "I") == 0 || strcmp(info[0], "D") == 0){
			
			added = updateList(head, info);
			if (added == NULL){ // the case of having updated the record or deletion of a non-existing node, continue reading the input.
				continue;
			}

			if (head == NULL){ //empty list, points head to the new added node
				head = added;
			}else{ // assume it is added to the correct position
				int hid,aid;
				hid = atoi(head->id);
				aid = atoi(added->id);
				if (aid < hid){  // change the head and point it to the added node that is infront of the previous head node
					head = added; 
				}else if(aid == hid && added != NULL){ // deletion of the original head, set the head to the next node/clean original head
					head = added->next;
					free(added);
					added = NULL;
				}else if (added != NULL && strcmp(info[1], added->id) == 0 && strcmp(info[0], "D") == 0){ // deletion of the middle node/clean it
					free(added);
					added = NULL;
				}
			}
		}else if (strcmp(info[0], "LIST") == 0){ //print the list to standard output
			printList(head);
		}else{
			puts("invalid command. Please Try Again");
		}
	}  
	return 0; // Appropriate return code from this program.
}


