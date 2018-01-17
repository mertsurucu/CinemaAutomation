#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <string.h>
#define true 1
#define false 0
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
static int hallCounter=-1;
static int soldBoolean=true,movieBoolean=true;
static char next[3]={'\0','\0','\0'};
char empty[]={'"','"'};
void createHall(char* request,int count,FILE *wf);
void buyTicket(char* request,int count,FILE *wf);
void cancelTicket(char* request,int count,FILE *wf);
void showHall(char* request,int count,FILE *wf);
char * nextChar(char*  current,FILE *wf);
void operation(int numInput,char* input[50],FILE *wf);
int searchForSeatLabel(char* key,int n,int hallNum,FILE *wf);
int searchForMovie(char* key,int n,FILE *wf);
int searchForHallName(char* key,int n,FILE *wf);
int putInOrder(char*seatLabel);
void statistics(char* request,FILE *wf);
int soldCheck(char* label,int num,int sold,FILE *wf);
char* splitForHall(char* word);
void writeScreen();
/*char readFile(){
	int i=0;
	int numProgs=0;
	char* input[50];
	char line[50];
	FILE *file;
	file = fopen("input.txt", "r");
	while(fgets(line, sizeof line, file)!=NULL) {
		//check to be sure reading correctly
		printf("%s", line);
		//add each filename into array of input
		input[i]=strdup(line);
		i++;
		//count number of input in file
		numProgs++;
	}
	//check to be sure going into array correctly
	fclose(file);
	return input;
}*/
/*
hall[0] "Red-Hall" "StarDust" 24 20
hall[1] "Orange-Hall" "Last_Samurai" 2
 */
typedef struct hall{
	char* hallName;
	char* movieName;
	int width;
	int height;
	char* seatLabel[200];/*it gets sold ones forEx"D4"*/
	int label;/*it counts total sold ticket numbers for each hall*/
	int sold;
}Hall;
Hall hall[20];
typedef struct seat{
	char *seatType[200];/*it gets sold ones forEx"Student"*/
}Seat;
Seat seat[20];

struct first{
	char* createHall;
	char* buyTicket;
	char* cancelTicket;
	char* showHall;
	char* statistics;
};	struct first req;

struct counter{
	int counterCH;
	int counterBT;
	int counterCT;
	int counterSH;
	int counterS;
};	struct counter c;
int main(int argc, char *argv[]){
	FILE *wf;
	wf=fopen("output.txt","w+");
	int i=0,m;
	int numProgs=0;
	char* input[50];
	char line[50];
	FILE *file;
	file = fopen(argv[1], "r");
	while(fgets(line, sizeof line, file)!=NULL) {
		/*check to be sure reading correctly
		add each filename into array of input*/
		input[i] = (char*)malloc(strlen(line)+1);
		strcpy(input[i],line);

		i++;
		/*count number of input in file*/
		numProgs++;
	}
	/*check to be sure going into array correctly*/
	fclose(file);
	operation(numProgs,input,wf);

	fclose(wf);
	writeScreen();
	return 0;
}
void writeScreen(){
FILE *file;
	int i=0;
	char* input[50];
	char line[50];
	file = fopen("output.txt", "r");
	while ((i = getc(file)) != EOF){

	        putchar(i);}

fclose(file);
	/*check to be sure going into array correctly*/


}
void operation(int numInput,char* input[50],FILE *wf){
	/*for controlling the fist word matches*/
	int m,count=0;
	for(m=0;m<numInput;m++){
		char *request;
		int temp;
		temp=count;
		request = strtok(input[m]," \n");
		c.counterCH=0;/*CREATEHALL komutunu aldiktan sonra hala ayni satirda oldugumuzu anlamak icin satir counteri*/
		c.counterBT=0;
		c.counterCT=0;
		c.counterSH=0;
		c.counterS=0;
		while (request != NULL)
		{
			/*printf ("%s\n",request);*/

			if(strcmp("CREATEHALL",request)== 0||c.counterCH!=0)/*CREATEHALL*/
			{	createHall(request,c.counterCH,wf);
			c.counterCH++;
			}
			else if(strcmp("BUYTICKET",request) == 0||c.counterBT!=0)
			{
				buyTicket(request,c.counterBT,wf);
				c.counterBT++;
				/*BUYTICKET*/
			}else	if(strcmp("CANCELTICKET",request) == 0||c.counterCT!=0)
			{	cancelTicket(request,c.counterCT,wf);
			c.counterCT++;
			/*CANCELTICKET*/
			}
			else if(strcmp("SHOWHALL",request) == 0||c.counterSH!=0)
			{
				showHall(request,c.counterSH,wf);c.counterSH++;
				/*SHOWHALL*/
			}
			else if(strcmp("STATISTICS",request) == 0||c.counterS!=0)
			{	statistics(request,wf);
			/*STATISTICS*/
			}
			request = strtok (NULL," \n");
		}
	}
}
void createHall(char* request,int count,FILE *wf){
	int i =0,j=0,width,height;
	switch(count){
	case 0:/*CREATEHALL*/
		hallCounter++; /*to create a new hall it getts new struct from Hall[]*/
		for (i=0; i <200; i++) {
			hall[hallCounter].seatLabel[i]=(char*)calloc(3,sizeof(char));
			seat[hallCounter].seatType[i]=(char*)calloc(10,sizeof(char));}
		hall[hallCounter].label=0;
		hall[hallCounter].sold=0;

		break;
	case 1:/*HALL_NAME*/
		hall[hallCounter].hallName=request;
		break;
	case 2:/*MOVIE_NAME*/
		hall[hallCounter].movieName=request;
		break;
	case 3:/*HALLWIDTH*/
		/*It gets width in integer*/
		width = atoi(request);
		if(width>26){
			fprintf(wf,"N/A\n");
			hall[hallCounter].hallName=(char*)malloc(0);
			hall[hallCounter].movieName=(char*)malloc(0);
			hall[hallCounter].hallName=(char*)malloc(30);
			hall[hallCounter].movieName=(char*)malloc(30);
			break;}
		else{hall[hallCounter].width=width;}
		break;
	case 4:/*HALLHEIGHT*/
		/*It gets width in integer*/
		height = atoi(request);
		hall[hallCounter].height=height;
		break;
	}
}
void buyTicket(char* request,int count,FILE *wf){
	static int num;
	int i,j=0,del;
	int temp,total;
	if(movieBoolean==true){
		switch(count){
		case 0:/*BUYTICKET*/
			break;
		case 1:/*MOVIENAME*/
			if(strcmp(empty,request) == 0){
				fprintf(wf,"ERROR: Movie name cannot be empty.\n");
				movieBoolean=false;
				break;}

			else{
				soldBoolean=true;
				num=searchForMovie(request,hallCounter+1,wf);
				if(num==-1){
					movieBoolean=false;
					break;}
			}
			/*to buy a ticket we need to match movie name and the struct Hall[]
			hall[searchForMovie(request,hallCounter+1)].seatLabel[];*/
			break;
		case 2:/*SEATLABEL D4 gelen sira numarasinin salonun icinde var oldugunu karsilastir*/
			temp=request[2]-'0';
			if(temp>0){
				temp=request[1]-'0';
				total+=(temp*10);
				temp=request[2]-'0';
				total+=temp;
			}else{
				temp=request[1]-'0';
				total+=temp;}
			if(soldCheck(request,num,hall[num].label,wf)==0){
				soldBoolean=false;
				fprintf(wf,"ERROR: Specified seat(s) in %s are not available! They have been already taken.\n",hall[num].hallName);
				break;
			}else if(hall[num].height<total||hall[num].width<request[0]-64){
				fprintf(wf,"ERROR: Seat %s is not defined at %s\n",request,hall[num].hallName);
				soldBoolean=false;
				break;
			}
			else{
				hall[num].seatLabel[hall[num].label]=request;
				hall[num].sold++;}
			/*label D4*/
			break;
		case 3:/*TicketType*/
			if(soldBoolean==false){break;}
			else{seat[num].seatType[hall[num].label]=request;}/*Biletin tipini aliyor ogrenci ya da adult odugu (1de)*/
			break;
		case 4:/*amount of seats*/
			if(soldBoolean==false){soldBoolean=true;break;}
			else{
				i = atoi(request);
				hall[num].label++;
				hall[num].sold=hall[num].sold+i-1;
				for(j;j<i-1;j++){
					if(soldBoolean==true){
						/*char *p=hall[num].seatLabel[hall[num].label-1];char next[3];char a[3];int m=0;for (m ; m <3 ; ++m) {a[1]=p[m];if(m==0){next[m]=p[m]+1;}else{
							next[m]=p[m];}*/
						char* p;
						p=(char*)malloc(3);
						p=nextChar(hall[num].seatLabel[hall[num].label-1],wf);
						if(soldCheck(p,num,hall[num].label,wf)==0){/*if we have sold same seat before*/
							soldBoolean=false;
							fprintf(wf,"ERROR: Specified seat(s) in %s are not available! They have been already taken.\n",hall[num].hallName);
							for (del = 0; del < j+1; ++del) {
								hall[num].seatLabel[hall[num].label-1-del]=NULL;
								seat[num].seatType[hall[num].label-1-del]=NULL;
							}hall[num].sold=hall[num].sold-i;
							break;}
						/*Seat is empty*/
						else{
							strcpy(hall[num].seatLabel[hall[num].label],p);
							seat[num].seatType[hall[num].label]=seat[num].seatType[hall[num].label-1];
							hall[num].label=hall[num].label+1;}}}
				if(soldBoolean==true){
					if(i==1){
						fprintf(wf,"%s %s Seat(s) %s successfully sold.\n",hall[num].hallName,hall[num].movieName,hall[num].seatLabel[hall[num].label-1]);}
					else if(i==2){
						fprintf(wf,"%s %s Seat(s) %s,%s successfully sold.\n",hall[num].hallName,hall[num].movieName,hall[num].seatLabel[hall[num].label-2],hall[num].seatLabel[hall[num].label-1]);}
					else if(i==3){
						fprintf(wf,"%s %s Seat(s) %s,%s,%s successfully sold.\n",hall[num].hallName,hall[num].movieName,hall[num].seatLabel[hall[num].label-3],hall[num].seatLabel[hall[num].label-2],hall[num].seatLabel[hall[num].label-1]);}
					else if(i==4){
						fprintf(wf,"%s %s Seat(s) %s,%s,%s,%s successfully sold.\n",hall[num].hallName,hall[num].movieName,hall[num].seatLabel[hall[num].label-4],hall[num].seatLabel[hall[num].label-3],hall[num].seatLabel[hall[num].label-2],hall[num].seatLabel[hall[num].label-1]);}
					else if(i==5){
						fprintf(wf,"%s %s Seat(s) %s,%s,%s,%s,%s successfully sold.\n",hall[num].hallName,hall[num].movieName,hall[num].seatLabel[hall[num].label-5],hall[num].seatLabel[hall[num].label-4],hall[num].seatLabel[hall[num].label-3],hall[num].seatLabel[hall[num].label-2],hall[num].seatLabel[hall[num].label-1]);}
				}
				soldBoolean=true;
				break;
			}
		}
	}if(count==4){/*Movie Name exeption forEx""*/
		movieBoolean=true;

	}}
void cancelTicket(char* request,int count,FILE *wf){
	int hallNum,seatNum;

	if(movieBoolean==true){
		switch (count) {
		case 0:/*CANCELTICKET*/
			break;
		case 1:/*Movie Name*/

			if(strcmp(empty,request) == 0){
				fprintf(wf,"ERROR: Movie name cannot be empty.\n");
				movieBoolean=false;

				break;}
			else{
				hallNum=searchForMovie(request,hallCounter+1,wf);
				if(hallNum==-1){
					movieBoolean=false;
					break;}
			}
			break;
		case 2:	/*Seat Label*/
			/*fprintf(wf,"\n%s\n",request);*/

			seatNum=searchForSeatLabel(request,hall[hallNum].label,hallNum,wf);
			if(seatNum==-1){fprintf(wf,"ERROR: Seat %s in %s was not sold.\n",request,hall[hallNum].hallName);}
			else{

				hall[hallNum].seatLabel[seatNum]=NULL;
				seat[hallNum].seatType[seatNum]=NULL;
				hall[hallNum].sold--;
				fprintf(wf,"%s %s Purchase cancelled. Seat %s is now free.\n",hall[hallNum].hallName,hall[hallNum].movieName,request);}
			break;
		}}
	if(count==2){/*Movie Name exeption forEx""*/
		movieBoolean=true;

	}
}
void showHall(char* request,int count,FILE *wf){
	int hallNum,h,w,b,alp,put,r,q,z;
	int seatOrder[100];
	char f='A';
	char *curtain=(char*)"C U R T A I N";
	switch (count) {
	case 0:/*SHOWHALL*/
		break;
	case 1:/*Hall_Name*/
		hallNum=searchForHallName(request,hallCounter+1,wf);
		for (r = 0; r < hall[hallNum].label+1; ++r) {
			if(hall[hallNum].seatLabel[r]!=NULL){
				seatOrder[r]=putInOrder(hall[hallNum].seatLabel[r]);}
		}
		fprintf(wf,"%s sitting plan\n",hall[hallNum].hallName);
		for (h = 0; h < hall[hallNum].height+1; ++h) {
			for (w = 0; w < (hall[hallNum].width*2)+3; ++w){
				if(w==0){
					fprintf(wf,"  ");fprintf(wf,"-");}
				else if(w!=0&&w <(hall[hallNum].width*2+1)) {
					fprintf(wf,"-");
				}
				else if(w==(hall[hallNum].width*2)+2){
					fprintf(wf,"\n");
				}if(w==(hall[hallNum].width*2)+2&&h==hall[hallNum].height){
					for (alp = 0; alp <hall[hallNum].width; ++alp) {
						if(alp==0){
							fprintf(wf,"   ");
						}
						fprintf(wf,"%c ",f+alp);
						if(alp==hall[hallNum].width){
							fprintf(wf,"\n");}
					}
					fprintf(wf,"\n");
				}if(w==(hall[hallNum].width*2)+2&&h==hall[hallNum].height){
					for (put = 0; put <hall[hallNum].width-6 ; ++put) {
						fprintf(wf," ");
					}
					fprintf(wf,"%s\n",curtain);
				}
			}if(h!=hall[hallNum].height){/*where we put | |s|*/
				for (b = 0; b < hall[hallNum].width+1; ++b) {
					if(b==0&&h<9){fprintf(wf,"%d ",h+1);}/*width order 1 to 9*/
					else if(b==0&&h>8){fprintf(wf,"%d",h+1);}/*height order 10 to 99*/
					fprintf(wf,"|");
					for (q = 0; q <hall[hallNum].label+1; ++q){
						if(seatOrder[q]==((b+1)*100)+h+1){
							if(strcmp(seat[hallNum].seatType[q],"Student")==0&&z!=1){
								fprintf(wf,"s");
								z=1;}
							else if(strcmp(seat[hallNum].seatType[q],"FullFare")==0&&z!=1){
								fprintf(wf,"f");
								z=1;}
						}
					}
					if(z!=1){fprintf(wf," ");}
					if(b==(hall[hallNum].width)){
						fprintf(wf,"\n");
					}z=0;
				}
			}

		}
		break;
	}
}
void statistics(char* request,FILE *wf){
	int hallNum,student,fullFare,i;
	fprintf(wf,"Statistics\n");
	for (hallNum = 0; hallNum <hallCounter+1; ++hallNum) {
		student=0;
		fullFare=0;
		for (i = 0; i < hall[hallNum].label+1; ++i) {
			/*printf("Statics;%d\nogrenci;%d;;;;;%s\n",i,student,seat[hallNum].seatType[i]);*/

			if (seat[hallNum].seatType[i]!=NULL&&strcmp(seat[hallNum].seatType[i],"FullFare")==0){
				fullFare++;
			}else if (seat[hallNum].seatType[i]!=NULL&&strcmp(seat[hallNum].seatType[i],"Student")==0&&seat[hallNum].seatType[i]!=NULL){
				student++;
			}
		}
		fprintf(wf,"%s %d student(s), %d full fare(s), sum:%d TL\n",hall[hallNum].movieName,student,fullFare,(fullFare*10)+(student*7));
	}
	/*STATISTICS*/
}
int putInOrder(char*seatLabel){
	int l,total=0,temp;
	if(seatLabel!=NULL){

		for(l=0;l<2;l++){

			if (l==0) {
				total=total+((seatLabel[l]-64)*100);

			}else if(l==1){
				temp=seatLabel[l+1]-'0';
				if(temp>0){
					temp=seatLabel[l]-'0';
					total+=(temp*10);
					temp=seatLabel[l+1]-'0';
					total+=temp;
				}else{
					temp=seatLabel[l]-'0';
					total+=temp;}
			}
		}/*printf("%s;%d\n",seatLabel,total);*/
	}
	return total;
}
char *nextChar(char* current,FILE *wf){

	char *a;

	int i=0;
	for (i ; i <3 ; ++i) {
		if(i==0){
			next[i]=current[i]+1;
		}else{
			next[i]=current[i];
		}

	}
	return  (char*)next;

}
int searchForMovie(char* key,int n,FILE *wf){/*SEARCHING for movie name*/
	int j=0,a=-1;
	for (j; j < n; j++){
		if(strcmp(hall[j].movieName,key)==0){/*ERROR*/
			a=j;
			break;
		}
	}if(a==-1){
		fprintf(wf,"ERROR: movie name %s is incorrect.\n",key);
		a=-1;
	}
	return a;
}int searchForHallName(char* key,int n,FILE *wf){
	int j=0,a;
	for (j; j < n; j++){
		if(strcmp(hall[j].hallName,key)==0){/*ERROR mesaji*/
			a=j;
			break;
		}
	}
	return a;
}
int searchForSeatLabel(char *key,int n,int hallNum,FILE *wf){
	int j=0,a;
	char newTemp[3]={'\0','\0','\0'};
	for(j;j<3;j++){
		newTemp[j]=key[j];
	}
	j=0;
	for (j ; j < n+1; j++){
		/*fprintf(wf,"##########%s;%s\n",newTemp,hall[hallNum].seatLabel[j]);*/
		if(hall[hallNum].seatLabel[j]!=NULL){
			if(strcmp(hall[hallNum].seatLabel[j],newTemp)==0){/*ERROR mesaji*/
				a=j;
				break;
			}else if(j==n||hall[hallNum].seatLabel[j]==NULL){
				/*fprintf(wf,"\nBoyle bi koltuk bulunamadi\n");*/
				a=-1;
			}}

	}
	/*fprintf(wf,"\ngonderilen numara %d\n",a);*/
	return a;
}int soldCheck(char* label,int num,int sold,FILE *wf){
	int j=0,a=true,i;

	for(j;j<sold+1;j++){
	
		if(hall[num].seatLabel[j]!=NULL&&strcmp(hall[num].seatLabel[j],label)==0){/*sold*/
			a=false;break;}	printf("\n",label,hall[num].seatLabel[j]);
	}
	return a;
}char* splitForHall(char* word){
	char* taken;
	taken=strtok(word,empty);
	return (char*) taken;
}







