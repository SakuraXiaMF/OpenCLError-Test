#include <CL/cl.h>
#include "tool.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdbool.h>
#include <string.h>
#include <fstream>
#include "defs.h"
#include <cstring>
#include <errno.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <utility>

//#include <process.h>
#define MAX_MSG_SIZE 256
#define SERVER_PORT 9987

using namespace std;


extern "C" void xlate_gpu(machine_t *m);
extern "C" int emit_normal(unsigned long next_eip, decode_t *d,
                           unsigned char *bbOut, unsigned char *bbLimit);
extern "C" int emit_jmp(unsigned long next_eip, decode_t *d,
                        unsigned char *bbOut, unsigned char *bbLimit);

bool bb_emit_byte(unsigned char *bbOut, unsigned char *bbLimit, unsigned char c)
{
    if (bbOut == bbLimit)
        return false;
    else
    {
        *bbOut = c;
        bbOut++;
    }
    return true;
}

int emit_normal(unsigned long next_eip, decode_t *d,
                unsigned char *bbOut, unsigned char *bbLimit)
{
    int i;
    //int count = next_eip - (int)d->decode_eip;
    int count = 1;
    for (i = 0; i < count; i++)
    {
        if (!bb_emit_byte(bbOut, bbLimit, *(d->instr)))
            return 0;
    }

    return 1;
}

int emit_jmp(unsigned long next_eip, decode_t *d,
             unsigned char *bbOut, unsigned char *bbLimit)
{
    int i;
    int count = next_eip - (int)d->decode_eip;
    for (i = 0; i < count; i++)
    {
        if (!bb_emit_byte(bbOut, bbLimit, *(d->instr)))
            return 0;
    }

    return 1;
}

bool translate_instr(unsigned long next_eip, decode_t *ds,
                     unsigned char *bbOut, unsigned char *bbLimit)
{
    return (emit_normal(next_eip, ds, bbOut, bbLimit));
}
//************************OpenCL********************************************************

unsigned char instr_sample[] = {
    0x5b, 0x55, 0x5d, 0xc9, 0xc3,
    0x90, 0x5b, 0x5d, 0x5e, 0x5f,
    0x50, 0x51, 0x52, 0x53, 0x54,
    0x55, 0x56, 0x50, 0xf4, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc2, 0xc3};

char instr_stream[testnum * INSTR_STREAM_SIZE + 2];

machine_t IA32;

void xlate_gpu(machine_t *m)
{
    cl_int status=0;
    /**Step 1: Getting platforms and choose an available one(first).*/
    cl_platform_id platform;
    getPlatform(platform);
   // printf("Trap????\n");
    /**Step 2:Query the platform and choose the first GPU device if has one.*/
    cl_device_id *devices = getCl_device_id(platform);
   // printf("Trap????\n");
    /**Step 3: Create context.*/
    cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL, &status);
     if (status != CL_SUCCESS)
    {
        printf("Create Context  Failed  status is %d\n",status);
        return;
    }
   // printf("Trap????\n");
    /**Step 4: Creating command queue associate with the context.*/
    cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, &status);
     if (status != CL_SUCCESS)
    {
        printf("Create CommandQueue Failed  status is %d\n",status);
        return;
    }
    /**Step 5: Create program object */
    const char *filename = "kernel.cl";
    string sourceStr;
    status = convertToString(filename, sourceStr);
    const char *source = sourceStr.c_str();
    size_t sourceSize[] = {strlen(source)};
    cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, NULL);
   // printf("Trap????\n");
    /**Step 6: Build program. */
    const char options[] = "-w -I ./";
    status = clBuildProgram(program, 1, devices, options, NULL, NULL);
    if (status != CL_SUCCESS)
    {
            printf("Build Program Failed Status is %d\n", status);
            size_t log_size;
            clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

            // Allocate memory for the log
            char *log = (char *)malloc(log_size);

            // Get the log
            clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

            // Print the log
            printf("OpenCL Log is :\n%s\n", log);
        return;
    }
    return;
}

int main(int argc, char **argv)
{
    printf("Start\n");
    machine_t *m;
    m = &IA32;
    xlate_gpu(m);
    printf("Finish\n");
    return 0;
}