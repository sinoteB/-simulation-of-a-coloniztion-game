
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cerrno>
#include <list>
#include <pthread.h>
#include <algorithm>
#include <unistd.h>
using namespace std;
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
//struct def
typedef struct myStruct_{
	int row;
	int col;
	int val;
}myStruct;
typedef struct myStructSS_{
	int row;
	int col;
	int allTeams;
	vector<pthread_t>  allThreads;
}myStructSS;

fstream outfile("map.bin", ios::out | ios::in | ios::binary);


//initialize team a
void putMembers(int teamA, int m, int n, int c){


	int a = c;
	for(int i = 0;i<teamA;i++){
		int p;
		p=rand()%(m*n);
		outfile.seekg(p);
		if(outfile.get()==1){
			outfile.seekp(p);
			outfile.put(a);
		}else{
			i--;
		}
	}


}

void display(int m, int n){

	fstream outfile("map.bin", ios::in | ios::binary);

	//print out the the content of map
	outfile.seekg(0);
	for(int i = 0 ; i < m ; i++){
		for(int j = 0; j < n ; j++){
			int pos = outfile.get();
			if(pos == 1){
				cout<<(char)1000<<" ";
			}else if(pos == 2){
				cout<<"\033[31m"<<"X "<<"\033[0m";
			}else if(pos == 3){
				cout<<"\033[34m"<<"O "<<"\033[0m";
			}else if(pos == 4){
				cout<<"\033[33m"<<"x "<<"\033[0m";
			}else if(pos == 5){
				cout<<"\033[36m"<<"o "<<"\033[0m";
			}

		}
		cout<<endl;
	}

}

//generate an array for the adresses of the blockes around a square
list<int> surroundings(int m, int n, int i, int j){
	list<int> r;
	int num = i*n+j;

	r.push_back(i*n+j);
	if(i-1>=0 && j-1>=0){
		num = (i-1)*n+(j-1);
		r.push_back(num);
	}

	if(i-1>=0){
		num = (i-1)*n+(j);
		r.push_back(num);
	}

	if(i-1>=0 && j+1<n){
		num = (i-1)*n+(j+1);
		r.push_back(num);
	}

	if(j-1>=0){
		num = (i)*n+(j-1);
		r.push_back(num);
	}

	if(j+1<n){
		num = (i)*n+(j+1);
		r.push_back(num);
	}

	if(i+1<m && j-1>=0){
		num = (i+1)*n+(j-1);
		r.push_back(num);
	}

	if(i+1<m){
		num = (i+1)*n+(j);
		r.push_back(num);
	}
	if(i+1<m && j+1<n){
		num = (i+1)*n+(j+1);
		r.push_back(num);
	}
	return r;
}

void * missile(void * pParam){

	myStruct * myParam = (myStruct *)pParam;
	int m = myParam->row;
	int n = myParam->col;
	int a = myParam->val;
	while(1){
	int i = -1;
	int j = -1;
	int x = 0;
	int o = 0;
	int waitTime =(rand()%3)+1;

	pthread_mutex_lock(&myMutex);

	while(1){
		i = rand()%m;
		j = rand()%n;
		outfile.seekg(i*n+j);
		int num = outfile.get();
		if(num!=2 && num!=3) break;
	}
	list<int> s = surroundings(m,n,i,j);
	int size = s.size();
	int r[size];

	copy(s.begin(),s.end(),r);
	outfile.seekg(r[0]);
	int current = outfile.get();


	if(current != a){

		outfile.seekp(r[0]);
		outfile.put(a);
	}else{
		outfile.seekp(r[0]);
		outfile.put(1);
	}
	if(a==4)cout<<"Team A attacking coordinates ("<<j<<" , "<<i<<")"<<endl;
	else if(a==5)cout<<"Team B attacking coordinates ("<<j<<" , "<<i<<")"<<endl;


	for(int t= 0; t<size; t++){
		outfile.seekg(r[t]);
		int pos = outfile.get();
		if(pos == 2 ||pos == 4){
			x++;
		}else if(pos == 3 || pos == 5){
			o++;
		}
	}
	if(x>o && a == 4){

		for(int t=1; t<size; t++){

			outfile.seekg(r[t]);
			int pos = outfile.get();
			if(pos == 1 || pos == 5){
				outfile.seekp(r[t]);
				outfile.put(4);
			}

		}
	}else if(o>x && a == 5){
		for(int t=0; t<size; t++){


			outfile.seekg(r[t]);
			int pos = outfile.get();
			if(pos == 1 || pos == 4){
				outfile.seekp(r[t]);
				outfile.put(5);
			}
		}

	}



	pthread_mutex_unlock(&myMutex);
	display(m,n);

	sleep(waitTime);
	}
	return (void*)0;
}

void * supervisor (void * pParam){
	bool space = true;
	myStructSS * myParam = (myStructSS *)pParam;
	int m = myParam->row;
	int n = myParam->col;
	int teams = myParam->allTeams;
	vector<pthread_t> threads = myParam->allThreads;

	while(space){

	pthread_mutex_lock(&myMutex);



	int size = m*n;
	space = false;

	for(int t=0; t<size;t++){
		outfile.seekg(t);
		int pos = outfile.get();
		if(pos == 1){
			space = true;
			break;
		}

	}
	if(!space){
		int x = 0;
		int o = 0;


		for(int t= 0; t<size; t++){
			outfile.seekg(t);
			int pos = outfile.get();
			if(pos == 2 ||pos == 4){
				x++;
			}else if(pos == 3 || pos == 5){
				o++;
			}
		}
		pthread_mutex_unlock(&myMutex);
		if(x>o){
			cout<<"Team A won with the score "<<x<<" x's to "<<o<<" o's"<<endl;
		}else if(o>x){
			cout<<"Team B won with the score "<<x<<" x's to "<<o<<" o's"<<endl;

		}else if(o==x){
			cout<<"It's a draw with the score "<<x<<" x's to "<<o<<" o's"<<endl;

		}


		for(int i = 0; i<teams; i++){
			pthread_cancel(threads[i]);
		}

	}else{

		pthread_mutex_unlock(&myMutex);

	}


	}
	return (void *)0;
}
//main function with argc for the size of the array and argv[] for the command line arguments
int main(int argc, char* argv[]){
	//check for the right amount of command line argument which should be 5
	if(argc!=5){
		cout<<"please use the following format when you run the program"<<endl;
		cout<<"[excutable][team A num][team B num][row][col]"<<endl;
		return 1;
	}
	string arg1 = argv[1];
	string arg2 = argv[2];
	string arg3 = argv[3];
	string arg4 = argv[4];


	int teamA;
	int teamB;
	int m;
	int n;


	//storing the command line arguments
	try{
		teamA = stoi(arg1);
		teamB = stoi(arg2);
		m = stoi(arg3);
		n = stoi(arg4);
	} catch (const invalid_argument& e){

		cout<<"Please enter an integer values"<<endl;
		return 1;
	}

	int teams = teamA + teamB;

	if(teamA < 1 || teamB < 1 || m < 1 || n < 1){
		cout<<"You have entered a value that is less than one please make sure to enter a positive integer"<<endl;
		return 1;

	}

	if( teams >= ( m*n)){
		cout<<"The size of your map is not sufficient for the amout of team members you have requested"<<endl;
		return 1;
	}
	//srand
	srand(time(NULL));

	//create and open a binary file called map for both reading and writing 
	//fstream outfile("map.bin", ios::out | ios::binary);

	//check if the map file is open
	if(!outfile.is_open()){
		perror("Failed to open file"); 
		return 1;
	}

	//initialize the map with 1 to indicate free land accoriding to the command line argument
	int free = 1;

	for(int i=0;i<n*m;i++){
		outfile.put(free);

	}

	//put the two team in the map
	putMembers(teamA, m, n, 2);
	putMembers(teamB, m, n, 3);
	cout<<"\033[32m"<<"Welcom to the lab3: simulation of a coloniztion game"<<"\033[0m"<<endl;
	cout<<"Uppercase X and O represent permanent locations of team A and team B respectively"<<endl;
	cout<<"Lowercase x and o represent conqeuered locations of Team A and Team B respectively"<<endl;
	display(m,n);

	myStruct * tA = new myStruct;
	tA->row=m;
	tA->col=n;
	tA->val=4;

	myStruct * tB = new myStruct;
	tB->row=m;
	tB->col=n;
	tB->val=5;

	pthread_t sThread;
	pthread_t threads [teams];
	pthread_mutex_init(&myMutex, NULL);
	for(int i = 0; i < teamA ; i++){
		pthread_create(&threads[i], NULL, missile, (void *) tA);

	}
	for(int i = 0; i < teamB ; i++){
		pthread_create(&threads[1], NULL, missile, (void *) tB);
	}



	vector<pthread_t> thread(threads, threads + teams);

	myStructSS * sS = new myStructSS;
	sS->row = m;
	sS->col = n;
	sS->allTeams = teams;
	sS->allThreads = thread;

	pthread_create(&sThread, NULL, supervisor, (void *) sS);
	
	//for (int i = 0; i < teams ; i++) {
		//cout<<threads[i]<<endl;

		//pthread_join(threads[i], NULL);
		//cout<<"joined "<<i<<endl;
	//}
	pthread_join(sThread, NULL);
	display(m,n);
	
	//close file
	outfile.close();

	return 0;
}

