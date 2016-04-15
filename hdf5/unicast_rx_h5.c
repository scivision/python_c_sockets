/* unicast IPv6 UDP array transmit
 Michael Hirsch
 based on https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <math.h>

// HDF5
#include "hdf5.h"
#define FILENAME "testc.h5"
static const int RANK=1;

static const int BUFSIZE=8192;
static const int Nloop=1000; // to not fill your hard drive!

void error(char *msg, int sock) __attribute__ ((noreturn));

void error(char *msg, int sock) {
    perror(msg);
    close(sock);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
//--------------------------------------------------------------------
// HDF5 stuff https://www.hdfgroup.org/ftp/HDF5/current/src/unpacked/examples/h5_extend.c
    hid_t       fid, dset, memspace_id,dataspace_id, prop;
    herr_t      status;
    hsize_t      chunk_dims[1] = {BUFSIZE};
    hsize_t      dims[1]  = {Nloop*BUFSIZE/4};   // dataset dimensions at creation time	
    hsize_t      maxdims[1] = {H5S_UNLIMITED};  // can write up to hard drive size
    hsize_t      dimslice[1] = {BUFSIZE/4}; //how many to write at once
    hsize_t     offset[1], count[1];

    // Create a new file. If file exists its contents will be overwritten.
    fid = H5Fcreate (FILENAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // Create the data space with unlimited dimensions. 
    dataspace_id = H5Screate_simple (RANK, dims, maxdims);

    // Modify dataset creation properties, i.e. enable chunking 
    prop = H5Pcreate (H5P_DATASET_CREATE);
    status = H5Pset_chunk (prop, RANK, chunk_dims);
    if (status<0){
        H5Eprint1(stderr);
        return EXIT_FAILURE;
    }
    // Create a new dataset within the file using chunk creation properties.
    // datatypes: https://www.hdfgroup.org/HDF5/doc/RM/PredefDTypes.html
    dset = H5Dcreate2 (fid, "/data", H5T_IEEE_F32LE, dataspace_id,
                                        H5P_DEFAULT, prop, H5P_DEFAULT);

    printf("writing data to %s\n",FILENAME);
//------------------------------------------------------------------------------    

    ssize_t ret;
    size_t Nel = BUFSIZE/4; // float32->4 bytes
    socklen_t serverlen;
    struct sockaddr_in6 serveraddr;
    struct addrinfo *server;

    char *hostname = "::1";
    int port=2000;

    
    unsigned long i=0;
    char buf[1]="\n";
    float * array;
    array = malloc(Nel*sizeof(float));

    if (argc>1) 
        hostname = argv[1];
    if (argc>2)
        port = atoi(argv[2]);

    /* socket: create the socket */
    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) 
        error("ERROR opening socket",s);

    ret = getaddrinfo(hostname,NULL,NULL,&server);

    /* build the server's Internet address */
    memset((char *) &serveraddr,0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_port = htons(port);

    
    bool first = true;
    float last=0.;
    // loop
    while (true)
    {
    // ask server for data (demo server expects simply a line return)
    serverlen = sizeof(serveraddr);
    ret = sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &serveraddr, serverlen);
    if (ret < 0) 
      error("ERROR in sendto",s);
    
    // get the length of data
    ret = recvfrom(s, &Nel, sizeof(Nel), 0, (struct sockaddr*) &serveraddr, &serverlen);
    if (ret < 0) 
      error("ERROR in recvfrom",s);
    

    // get the data
    ret = recvfrom(s, array, BUFSIZE, 0, (struct sockaddr*) &serveraddr, &serverlen);
    if (ret < 0) 
      error("ERROR in recvfrom",s);

    // check the data
    if (first) {
        first = false;
        last = array[0]-1;
        printf("Initial last: %f\n",last);
    }

    if (fabsf(array[0]-(float)1.)<0.99){
        printf("last: %f data: %f\n",last,array[0]-1);
        printf("may be wrapping in float32");
        return EXIT_FAILURE;
    } 


    last = array[Nel-1];

    //------------------------------------------------------------------------
    // Select a hyperslab in dataset 
    memspace_id = H5Screate_simple (RANK, dimslice, NULL); 
    dataspace_id = H5Dget_space (dset);
    offset[0] = i*Nel; // where to start writing
    count[0] = Nel; // how many to write

    status = H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, offset, NULL, count, NULL);  
    if (status<0){
        H5Eprint1(stderr);
        return EXIT_FAILURE;
    }

    // Write data to dataset
    status = H5Dwrite (dset, H5T_IEEE_F32LE, memspace_id, dataspace_id, H5P_DEFAULT, array);
    if (status<0){
        H5Eprint1(stderr);
        return EXIT_FAILURE;
    }

    i++;

    if (i==Nloop){
    printf("this concludes this test writing %s\n",FILENAME);
    break;
    }
    }

    status = H5Sclose (dataspace_id);
    status = H5Dclose (dset);
    status = H5Fclose (fid);




    return EXIT_SUCCESS;
}

