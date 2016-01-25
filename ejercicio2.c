#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void Get_input(unsigned long long int* number_of_tosses, int my_rank, MPI_Comm comm);
unsigned long long int Monte_carlo(unsigned long long int number_of_tosses, int my_rank);

int main(int argc, char *argv[]) {
  unsigned long long int number_of_tosses;
  unsigned long long int local_number_of_tosses;
  unsigned long long int number_in_circle;
  unsigned long long int local_number_in_circle;

  double pi_estimate;
  int my_rank;
  int comm_sz;
  MPI_Comm comm;

  MPI_Init(&argc, &argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &comm_sz);
  MPI_Comm_rank(comm, &my_rank);

  Get_input(&number_of_tosses, my_rank, comm);

  local_number_of_tosses = number_of_tosses/comm_sz;
  number_of_tosses = local_number_of_tosses * comm_sz;
  local_number_in_circle = Monte_carlo(local_number_of_tosses,my_rank);
  MPI_Reduce(&local_number_in_circle,&number_in_circle,1,MPI_UNSIGNED_LONG_LONG,MPI_SUM,0,comm);

  if(my_rank == 0) {
    pi_estimate = 4 * ((double) number_in_circle) / ((double) number_of_tosses);
    printf("Value of pi: %f\n",pi_estimate);
  }

  MPI_Finalize();
  return 0;
}


void Get_input( unsigned long long int* number_of_tosses,
                int my_rank,
                MPI_Comm comm) {
  if(my_rank == 0) {
    printf("Count: ");
    scanf(number_of_tosses);
  }
  MPI_Bcast(number_of_tosses,1,MPI_UNSIGNED_LONG_LONG,0,comm);
}  
unsigned long long int Monte_carlo(unsigned long long int local_number_of_tosses, int my_rank) {
  unsigned long long int result = 0;
  double radius = 1;
  unsigned long long int i;
  for(i = 0; i < local_number_of_tosses; i++) {
    double x = ((double) rand()) / ((double) RAND_MAX) * radius * 2;
    double y = ((double) rand()) / ((double) RAND_MAX) * radius * 2;
    int dist = sqrt(x * x + y * y);
    if(dist <= 1) result ++;
  }
  return result;
}
