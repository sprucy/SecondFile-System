#include "Buffer.h"
#include <iostream>
#include <iomanip>
#include <ctype.h>
using namespace std;

Buffer::Buffer() {
	flags = 0;
	forw = NULL;
	back = NULL;
	wcount = 0;
	addr = NULL;
	blkno = -1;
	u_error = -1;
	resid = 0;

    no=0;
}

Buffer::~Buffer() {

}


