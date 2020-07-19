#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------------------
// to compile this program:
// bash> g++ -std=c++11 -o raid4 raid4.c
// ----------------------------------------------------------------------------------------
// download the input file:
// bash> curl https://apeters.web.cern.ch/apeters/pd2020/raid4.input -o raid4.input
// ----------------------------------------------------------------------------------------
// run the program (in the same directory with the input file)
// bash> ./raid4 
// ----------------------------------------------------------------------------------------

int main()
{
  int ifd;            // input file filedescriptor
  int fd[5];          // array of output file filedescriptor
  unsigned char q[5]; // array of column-wise parity

  // open input,output files
  ifd = open("raid4.input",0);
  if (ifd<0) {
    fprintf(stderr,"error:failed to open input file\n");
    exit(-1);
  }
  for (size_t i = 0 ; i < 5; i++) {
    // build the file name raid.0 ... raid.4
    std::string name="raid4.";
    name += std::to_string(i);
    
    // creat the output files
    fd[i] = open(name.c_str(), O_CREAT|O_RDWR, S_IRWXU);

    if (fd[i]<0) {
      fprintf(stderr,"error:failed to open output file %s\n", name.c_str());
      exit(-1);
    }
    // show the output files
    printf("%s fd[%d]=%d\n", name.c_str(), i, fd[i]);

    // reset the colum-wise parity
    q[i] = 0;
  }

  size_t nread=0;          // bytes read in a read call
  unsigned char buffer[5]; // 4 bytes for input bytes, 1 byte for output bytes


  // ----------------------------------------------------------------------------------------
  // we are reading now in a loop the input file from beginnig to end in blocks of four bytes
  // ----------------------------------------------------------------------------------------
  do {
    // zero input buffer
    memset(buffer,0, 5);
    // read 4 bytes at a time
    nread = read(ifd, buffer,4);
    // -------------------------------------------------------------------------
    // IMPLEMNT THIS for 1.2
    // -------------------------------------------------------------------------
    // compute row-wise parity and store in buffer[4]
    // -------------------------------------------------------------------------

    if (nread) { // if we still read atleast one byte ...
      // write out the bytes, byte 0 to raid4.0 .... byte 4 to raid4.4 
      for (size_t i = 0; i < 5; i++) {
	write(fd[i],&buffer[i],1);
	// -------------------------------------------------------------------------
	// IMPLEMNT THIS for 1.3
	// -------------------------------------------------------------------------
	// compute column-wise and store in q[0] ... q[5] ...
	// -------------------------------------------------------------------------
      }
    }
  } while (nread == 4); // run until

  // print column-wise file parity
  printf("parity q: 0 x %02x %02x %02x %02x %02x\n", q[0],q[1],q[2],q[3],q[4]);
  // close input,output files
  close (ifd);
  for (size_t i = 0 ; i < 5; i++) {
    close(fd[i]);
  }
}
