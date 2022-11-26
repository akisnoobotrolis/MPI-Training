#include <iostream>
#include "mpi.h"
#include <cstdio>
#include <stdlib.h>

using namespace std;


int checkIfArrayIsSorted(int * , int , int, int &);
void findFirstMistake(int, int );

void showArray(int *, int );



int main(int argc, char* argv[]) {

	
	int rank, nproc;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);


	findFirstMistake(rank,nproc);

	
	
	


	MPI_Finalize();



	return 0;
}







void showArray(int* myArr, int size) {
	cout << "-----\n";
	for (int i = 0; i < size; i++)
	{

		if (i<size-1)
		{
			cout << myArr[i] <<" ";

		}
		else
		{
			cout << myArr[i] <<"\n-----\n"<< endl;
		}
		
	}
	
	
}





	int checkIfArrayIsSorted(int* myArr, int end,int currentRank,int  &value) {
		
		cout << "Im proccess " << currentRank << " and my sub array is:" << endl;
		
		showArray(myArr, end);
		
		cout << "Im proccess " << currentRank << " and i will start comparing\n";
		
		if (end-1==0)
		{
			return -1;
		}
		int startingPos = 1;

		while (startingPos < end)
		{
	
			if (myArr[startingPos - 1] >= myArr[startingPos])
			{
				value = myArr[startingPos - 1];
				cout <<"I am proccess "<< currentRank<< ". "<< myArr[startingPos - 1] << " is bigger than " << myArr[startingPos] << ". I send position " << startingPos - 1 << " of the sub array and its value "<< value <<endl << endl;
				return startingPos - 1;
			}
			startingPos++;
		}
		cout << "Im proccess "<< currentRank << " and my sub array is sorted. Sending back - 1\n\n";
		return -1;
	}



	void findFirstMistake(int rank, int nproc) {
		int value;
		int divi;
		int mod;
		int* myArr=NULL;
		int* temp;
		int size;
		int split;
		int firstSplit;
		int pos;
		int problemPos;
		MPI_Status status;
		if (rank==0)
		{

			cout << "Enter size of the Array\n";

			cin >> size;

			myArr = new int[size];


			for (int i = 0; i < size; i++)
			{
				cout << "Enter value for the " << i + 1 << " element\n";
				cin >> myArr[i];
			}


			cout << "Done\n";
			
			cout << "My Array is:\n\n";

			showArray(myArr, size);
			

			cin.ignore();
			cout << "press Enter continue";
			cin.ignore();
			cout << "\n\n------------------INFO------------------\n\n";
			


			
			firstSplit = div(size, (nproc - 1)).quot;
			mod= div(size, (nproc - 1)).rem;
		
			split = firstSplit;
			pos = 0;
			problemPos = -1;
			if (mod==0)
			{
				cout << "Number of proccesses with the size of the array have mod " << mod << endl<< endl <<endl;


			


				for (int i = 1; i < nproc; i++)
				{

					if (i == 2)
					{
						split = split++;
					}

					MPI_Send(&split, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
					temp = &myArr[pos];

					cout << "im proccess 0 and i send array with address " << temp << " and size " << split << " to proccess " << i << endl << endl;
					MPI_Send(temp, split, MPI_INT, i, 1, MPI_COMM_WORLD);



					pos = pos + split - 1;

				}
			}
			else if (nproc>size)
			{
				int x = 0;
				cout << "Number of proccesses is bigger than the size of the array"<< endl << endl << endl;
				split = 1;
				for (int i = 1; i < nproc; i++)
				{
					if (i == 2)
					{
						split = split++;
					}
					if (i < size+1) {
						
						MPI_Send(&split, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
						temp = &myArr[pos];

						cout << "im proccess 0 and i send array with address " << temp << " and size " << split << " to proccess " << i << endl << endl;
						MPI_Send(temp, split, MPI_INT, i, 1, MPI_COMM_WORLD);
					}
					else
					{
					
						MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

					}
					pos = pos + split - 1;
				}
				firstSplit = 1;
				split = 2;
			}
			else if (nproc<size)
			{
				
				split++;
				for (int i = 1; i < nproc; i++)
				{
					if (i==mod+1)
					{
						split--;
					}
					if (i == 2)
					{
						split = split++;
					}

					MPI_Send(&split, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
					temp = &myArr[pos];

					cout << "im proccess 0 and i send array with address " << temp << " and size " << split << " to proccess " << i << endl << endl;
					MPI_Send(temp, split, MPI_INT, i, 1, MPI_COMM_WORLD);



					pos = pos + split - 1;

				}
			}
			
			
			int i = 1;
			while (i < nproc && problemPos == -1)
			{
				
				MPI_Recv(&problemPos, 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status);
				if (problemPos!=-1)
				{
					MPI_Recv(&value, 1, MPI_INT, i, 4, MPI_COMM_WORLD, &status);
				}
				i++;
				
			}
			i--;

			for (int i = 0; i < nproc*nproc; i++)
			{
				cout << "";
			}
			cout << "\n\n---------------------------RESULT---------------------------\n\n\n";
			if (problemPos>-1 && i>1)
			{
				
				cout << "\t\tPROBLEM IS IN POSITION " << (i - 2) * split + problemPos - (i - 1) + firstSplit<<" WITH VALUE "<< value<<"\n\n------------------------------------------------------------\n\n\n" ;
				
			}
			else if(i == 1 && problemPos> -1)
			{
				cout << "\t\tPROBLEM IS IN POSITION " << problemPos << " WITH VALUE " << value << " \n\n------------------------------------------------------------\n\n\n" <<endl;
				
			}
			else
			{
				cout << "\t\t  THE ARRAY IS SORTED\n\n\n\n------------------------------------------------------------\n\n";
				
			}
			
			
			
		}
		else
		{

		
		
				MPI_Recv(&split, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				if (split>0)
				{
					myArr = new int[split];

					MPI_Recv(myArr, split, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
					cout << "im proccess " << rank << " and i receive sub array with size " << split << " from proccess 0 and saved it on the address " << myArr << endl << endl;

					problemPos = checkIfArrayIsSorted(myArr, split, rank, value);
					MPI_Send(&problemPos, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
					if (problemPos!=-1)
					{
						MPI_Send(&value, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
					}
				}
				else
				{
					
					problemPos = -1;
					MPI_Send(&problemPos , 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
				}
				
		
					
		}


	}



	







