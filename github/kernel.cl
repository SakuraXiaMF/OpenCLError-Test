#include <kerneldefs.h>
// This file contains definitions used in dbt-gpu.c, kernel.cu
//

bool do_decode(unsigned long next_eip, decode_t* ds,__global OpCode *nopbyte0_d)  // host
{
    //unsigned i;
    __global const OpCode *table = nopbyte0_d;
    __global const OpCode *pEntry = 0;
    //int wierd = 0;

    ds->decode_eip = next_eip;
    ds->attr = 0;
    ds->emitfn = 0;
    ds->modrm_regs = 0;
    //ds->need_sib = 0;
    //ds->dispBytes = 0;
    ds->no_of_prefixes = 0;
    ds->flags = 0;

    for(;;) {
	ds->b = (* ((unsigned char *) next_eip));
	if (ds->b > 255u)
	    goto handle_ifault;

	ds->attr = table[ds->b].attr;

	if ((ds->attr & DF_PREFIX) == 0)
	    break;

	ds->no_of_prefixes ++;

	switch(ds->b) {
	    case PREFIX_LOCK:
	    case PREFIX_REPZ:
	    case PREFIX_REPNZ:
		ds->flags |= DSFL_GROUP1_PREFIX;
		ds->Group1_Prefix = ds->b;
		break;

	    case PREFIX_CS:
	    case PREFIX_DS:
	    case PREFIX_ES:
	    case PREFIX_FS:
	    case PREFIX_GS:
	    case PREFIX_SS:
		ds->flags |= DSFL_GROUP2_PREFIX;
		ds->Group2_Prefix = ds->b;
		break;

	    case PREFIX_OPSZ:
		ds->flags |= DSFL_GROUP3_PREFIX;
		ds->opstate ^= OPSTATE_DATA32;
		ds->Group3_Prefix = ds->b;
		break;

	    case PREFIX_ADDRSZ:
		ds->flags |= DSFL_GROUP4_PREFIX;
		ds->opstate ^= OPSTATE_ADDR32;
		ds->Group4_Prefix = ds->b;
		break;
	}

	(next_eip)++;
    } 

    ds->instr = (unsigned char *)next_eip;

    for(;;) {
	ds->b = (* ((unsigned char *) next_eip));
	if (ds->b > 255u)
	    goto handle_ifault;
	pEntry = &table[ds->b];
	ds->attr = pEntry->attr;
	(next_eip)++;

	if (ds->attr & DF_TABLE) {
	    table = (__global OpCode*)pEntry->ptr;//(const OpCode*)pEntry->ptr;
	    //ds->attr &= ~DF_TABLE;
	    continue;
	}

	if (ds->attr & DF_FLOAT) {
	    //wierd = 1;
	    table = (__global OpCode*)pEntry->ptr;
	    ds->b = (* ((unsigned char *) next_eip));
	    if (ds->b > 255u)
		goto handle_ifault;
	    (next_eip)++;
	    ds->modrm.byte = ds->b;
	    if (ds->modrm.mod == 0x3u)
	    {
		pEntry = &table[ds->modrm.reg + 8];
		ds->attr = pEntry->attr;
		if (ds->attr & DF_ONE_MORE_LEVEL)
		{
		    table = (__global OpCode*)pEntry->ptr;//(const OpCode*)pEntry->ptr;
		    pEntry = &table[ds->modrm.rm];
		    ds->attr = pEntry->attr;
		}
	    }
	    else
	    {
		pEntry = &table[ds->modrm.reg];
		ds->attr = pEntry->attr;
	    }
	}

	break;
    }

    /* a lot of branches */

    ds->pInstr = (unsigned char *)next_eip;
    ds->emitfn =  pEntry->ptr;//ds->emitfn = (EMITFN)pEntry->ptr;
    ds->pEntry = (__global OpCode*)pEntry;

    if (ds->attr & DF_UNDEFINED)
	return false;

    if (pEntry->ptr == 0)
        return false;

    return true;

handle_ifault:
    ds->pInstr = (unsigned char *)next_eip;
    ds->emitfn = pEntry->ptr;
    ds->pEntry = (__global OpCode*)pEntry;

    return false;
} 

 bool bb_emit_byte(	 __global unsigned char* bbOut,  __global unsigned char* bbLimit,  unsigned char c)
{
    if (bbOut == bbLimit)
	return false;
    else {
        *bbOut = c;
        bbOut++;
    }
    return true;
} 


 int emit_normal( unsigned long next_eip, decode_t* d,
			 __global unsigned char* bbOut,  __global unsigned char* bbLimit)
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


 int emit_jmp( unsigned long next_eip,  decode_t* d, 
	                __global unsigned char* bbOut,__global unsigned char* bbLimit)
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

bool translate_instr( unsigned long next_eip,  decode_t* ds, 
	 __global unsigned char* bbOut,  __global unsigned char* bbLimit)
{
    //return (ds->emitfn)(next_eip, ds, bbOut, bbLimit);
    return (emit_normal(next_eip, ds, bbOut, bbLimit));
}

__kernel void xlate_kernel(__global char* instr_stream, __global unsigned char* code_cache,__global OpCode *nopbyte0_d)// 改
{
    unsigned long next_eip=0;
    decode_t d;
    next_eip = (unsigned long)instr_stream + ( get_global_id(0) );
    do_decode(next_eip, &d,nopbyte0_d);

    __global unsigned char* bbOut = code_cache + ( get_global_id(0) );
    __global unsigned char* bbLimit = code_cache + CODE_CACHE_SIZE;
    translate_instr(next_eip, &d, bbOut, bbLimit);
}
