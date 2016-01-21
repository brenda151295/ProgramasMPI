#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


int main() {
  int comm_sz, i, my_rank;
  

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int buffer = 0;
  if(comm_sz != 2)
  {
     MPI_Finalize();
     return 0;
  }

  if(my_rank == 0)
  { 
 	for(i=0 ; i<10 ; i++)
	{
	      MPI_Send(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	      printf("Sent to process 1 - value =%d \n", buffer);
	      buffer++;     
	      MPI_Recv(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	      printf("Received from process 1 - value = %d \n", buffer);
	}
  }
  else
  {
	for(i=0 ; i<10 ; i++)
	{
	      MPI_Recv(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	      printf("Received from process 0 - value = %d \n", buffer);
	      buffer++;
	      MPI_Send(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	      printf("Sent to process 0 - value = %d \n", buffer);
	}
  }

  MPI_Finalize();

  return 0;
}
