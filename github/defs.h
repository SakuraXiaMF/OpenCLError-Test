// This file contains definitions used in dbt-gpu.c, kernel.cu
// 

#ifndef _DEFS_H_
#define _DEFS_H_

#define INSTR_STREAM_SIZE (4096)
#define CODE_CACHE_SIZE (4096)

#define DF_NONE       0x00u
#define DF_MODRM      0x01u     /* Has mod/rm field */

#define DF_Ib         0x02u     /* Has immediate operand byte */
#define DF_Jb         0x02u     /* Has byte branch offset */

#define DF_Iv         0x08u     /* Has mode-dependent operand immediate */

#define DF_Jv         0x10u     /* Has mode-dependent address immediate */
#define DF_rel        0x10u     /* Has mode-dependent address immediate */

#define DF_Ob         0x40u     /* Has byte offset */
#define DF_Ov         0x80u     /* Has mode-dependent address immediate */

#define DF_PREFIX     0x100u    /* opcode prefix byte */

#define DF_UNDEFINED  0x200u    /* undefined opcode */
#define DF_TABLE      0x400u
#define DF_GROUP      0x800u

#define DF_r8         0x1000u   /* reg8 has unusual modrm decoding */
#define DF_rm8        0x2000u   /* rm8 has unusual modrm decoding */

#define DF_Iw         0x4000u   /* mode-independent imm16 */

#define DF_Ap         0x8000u   /* 4 byte or 6 byte direct address operand */

/* Following flags are for the use of floating-point operations only */
#define DF_FLOAT             0x10000u   /* floating point opcode */
#define DF_ONE_MORE_LEVEL    0x20000u    /* Needs an additional level of indexing */
#define DF_BINARY            0x40000u   /* Is it a binary floating-point operation? */
#define DF_DIRECTION         0x80000u    /* ST(0), ST(i) --> 0; ST(i), ST(0) --> 1 */

#define DF_ADDRSZ_PREFIX     0x100000u  /* Is the Address-Size Override Prefix Legal for this instruction */
#define DF_OPSZ_PREFIX       0x200000u   /* Is the Operand-Size Override Prefix Legal for this instruction */

#define MODE(x) (x)

/* decoding modes: */
#define b_mode MODE(0x1u)
#define v_mode MODE(0x2u)
#define w_mode MODE(0x3u)
#define d_mode MODE(0x4u)
#define p_mode MODE(0x5u)
#define s_mode MODE(0x6u)
#define q_mode MODE(0x7u) /* Quadword (64-bit) or Double Real */
#define x_mode MODE(0x7u) /* 98 / 108 bytes */
#define y_mode MODE(0x8u) /* extended real or BCD decimal (80-bit) */
#define z_mode MODE(0x9u) /* m14/28 bytes */

#define reg_AH  MODE(0x80u)
#define reg_AL  MODE(0x81u)
#define reg_BH  MODE(0x82u)
#define reg_BL  MODE(0x83u)
#define reg_CH  MODE(0x84u)
#define reg_CL  MODE(0x85u)
#define reg_DH  MODE(0x86u)
#define reg_DL  MODE(0x87u)
#define reg_DX  MODE(0x88u)
#define reg_indirDX  MODE(0x89u)        /* decoding hack */

#define reg_EAX MODE(0x90u)
#define reg_EBX MODE(0x91u)
#define reg_ECX MODE(0x92u)
#define reg_EDX MODE(0x93u)
#define reg_ESP MODE(0x94u)
#define reg_EBP MODE(0x95u)
#define reg_EDI MODE(0x96u)
#define reg_ESI MODE(0x97u)

#define reg_ES  MODE(0x0u)
#define reg_CS  MODE(0x1u)
#define reg_SS  MODE(0x2u)
#define reg_DS  MODE(0x3u)
#define reg_FS  MODE(0x4u)
#define reg_GS  MODE(0x5u)

#define ADDR_none    MODE(0u)
#define ADDR_implied_reg MODE(0x1u)
#define ADDR_E       MODE(0x2u)
#define ADDR_G       MODE(0x3u)
#define ADDR_imm     MODE(0x4u)
#define ADDR_es      MODE(0x5u)
#define ADDR_ds      MODE(0x6u)
#define ADDR_jmp     MODE(0x7u)
#define ADDR_seg     MODE(0x8u)
#define ADDR_seg_reg MODE(0x9u)
#define ADDR_direct  MODE(0xau)
#define ADDR_offset  MODE(0xbu)
#define ADDR_FREG    MODE(0xcu)

#define ADDR_R MODE(0xeu) /* mod field of modR/M must name a register */
#define ADDR_C MODE(0xfu) /* reg field of modR/M names a control register*/
#define ADDR_D MODE(0x10u) /* reg field of modR/M names a debug register*/
#define ADDR_T MODE(0x11u) /* reg field of modR/M names a test register*/

#define REG_EAX         0x0u
#define REG_ECX         0x1u
#define REG_EDX         0x2u
#define REG_EBX         0x3u
#define REG_ESP         0x4u
#define REG_EBP         0x5u
#define REG_ESI         0x6u
#define REG_EDI         0x7u
      
#define PREFIX_REPZ     0xf3u
#define PREFIX_REPNZ    0xf2u
#define PREFIX_LOCK     0xf0u
    
#define PREFIX_CS       0x2eu
#define PREFIX_DS       0x3eu
#define PREFIX_ES       0x26u
#define PREFIX_FS       0x64u
#define PREFIX_GS       0x65u
#define PREFIX_SS       0x36u
      
#define PREFIX_OPSZ     0x66u
#define PREFIX_ADDRSZ   0x67u


#define AH  ADDR_implied_reg, reg_AH
#define AL  ADDR_implied_reg, reg_AL
#define BH  ADDR_implied_reg, reg_BH
#define BL  ADDR_implied_reg, reg_BL
#define CH  ADDR_implied_reg, reg_CH
#define CL  ADDR_implied_reg, reg_CL
#define DH  ADDR_implied_reg, reg_DH
#define DL  ADDR_implied_reg, reg_DL
#define DX  ADDR_implied_reg, reg_DX
#define indirDX  ADDR_implied_reg, reg_indirDX

#define eAX ADDR_implied_reg, reg_EAX
#define eBP ADDR_implied_reg, reg_EBP
#define eBX ADDR_implied_reg, reg_EBX
#define eCX ADDR_implied_reg, reg_ECX
#define eDI ADDR_implied_reg, reg_EDI
#define eDX ADDR_implied_reg, reg_EDX
#define eSI ADDR_implied_reg, reg_ESI
#define eSP ADDR_implied_reg, reg_ESP

#define EAX ADDR_implied_reg, reg_EAX
#define EBP ADDR_implied_reg, reg_EBP
#define EBX ADDR_implied_reg, reg_EBX
#define ECX ADDR_implied_reg, reg_ECX
#define EDI ADDR_implied_reg, reg_EDI
#define EDX ADDR_implied_reg, reg_EDX
#define ESI ADDR_implied_reg, reg_ESI
#define ESP ADDR_implied_reg, reg_ESP

#define CS  ADDR_seg, reg_CS
#define DS  ADDR_seg, reg_DS
#define ES  ADDR_seg, reg_ES
#define FS  ADDR_seg, reg_FS
#define GS  ADDR_seg, reg_GS
#define SS  ADDR_seg, reg_SS

#define Ap  ADDR_direct, p_mode
#define Eb  ADDR_E, b_mode
#define Ev  ADDR_E, v_mode
#define Ep  ADDR_E, p_mode
#define Ew  ADDR_E, w_mode
#define Gb  ADDR_G, b_mode
#define Gv  ADDR_G, v_mode
#define Gw  ADDR_G, w_mode
#define Ib  ADDR_imm, b_mode
#define Iv  ADDR_imm, v_mode
#define Iw  ADDR_imm, w_mode
#define Jb  ADDR_jmp, b_mode
#define Jv  ADDR_jmp, v_mode
#define M   ADDR_E, 0
#define Ma  ADDR_E, v_mode
#define Mb  ADDR_E, b_mode
#define Md  ADDR_E, d_mode
#define Mp  ADDR_E, p_mode
#define Mq  ADDR_E, q_mode
#define Ms  ADDR_E, s_mode
#define Mw  ADDR_E, w_mode
#define Mx  ADDR_E, x_mode
#define My  ADDR_E, y_mode
#define Mz  ADDR_E, z_mode
#define Ob  ADDR_offset, b_mode
#define Ov  ADDR_offset, v_mode
#define Rd  ADDR_R, d_mode		/* ?? */
#define Cd  ADDR_C, d_mode		/* ?? */
#define Dd  ADDR_D, d_mode		/* ?? */
#define Td  ADDR_T, d_mode		/* ?? */
#define Sw  ADDR_seg_reg, w_mode
#define Xb  ADDR_ds, reg_ESI
#define Xv  ADDR_ds, reg_ESI
#define Yb  ADDR_es, reg_EDI
#define Yv  ADDR_es, reg_EDI

#define FREG ADDR_FREG, 0

#define NONE 0, 0

#define PREFIX(s) s, NONE, NONE, NONE, 0, "(prefix)", DF_PREFIX
#define RESERVED  "(reserved)", NONE, NONE, NONE, 0, "(reserved)", DF_UNDEFINED
#define GROUP(nm) "(group)", NONE, NONE, NONE, (void *)nm, #nm, DF_GROUP
#define TABLE(nm) "(table)", NONE, NONE, NONE, (void *)nm, #nm, DF_TABLE
#define FLOAT(nm) "(float)", NONE, NONE, NONE, (void *)nm, #nm, DF_FLOAT
#define FTABLE(nm) "(float)", NONE, NONE, NONE, (void *)nm, #nm, DF_ONE_MORE_LEVEL

#define EMIT(nm) ((void*)emit_##nm), "emit_" #nm

#define XX	0x0u
#define AX	DF_ADDRSZ_PREFIX
#define XO	DF_OPSZ_PREFIX
#define AO	DF_ADDRSZ_PREFIX | DF_OPSZ_PREFIX

/* Flag effect declaration */
#define N 0x0u	//No effect
#define R 0x1u	// Reads atleast one flag
#define W 0x2u	//Modifies at least one flag
#define RW 0x3u	//Reads atleast one flag and writes at least one flag
#define ND W	// Non-Deterministic, as per manual
#define U RW	//Unsure
#define FL N	//Floating Point
#define SSE N	//SSE/SSE2

#define MMX_SSE_SSE2            "MMX/SSE/SSE2", M,    NONE, NONE, EMIT(normal), AX, SSE
#define MMX_SSE_SSE2_Ib         "MMX/SSE/SSE2", M,    Ib,   NONE, EMIT(normal), AX, SSE
#define MMX_SSE_SSE2_None       "MMX/SSE/SSE2", NONE, NONE, NONE, EMIT(normal), AX, SSE

#define DSFL_GROUP1_PREFIX    0x1u
#define DSFL_GROUP2_PREFIX    0x2u
#define DSFL_GROUP3_PREFIX    0x4u
#define DSFL_GROUP4_PREFIX    0x8u

#define OPSTATE_ADDR32 0x1u
#define OPSTATE_DATA32 0x2u

#define OPSTATE_ADDR16 0x0u
#define OPSTATE_DATA16 0x0u


#define MODETYPE unsigned char

#define testnum 50000

typedef struct machine_s {
	unsigned long next_eip;
	
	unsigned char code_cache[testnum*CODE_CACHE_SIZE+2];
	unsigned char *bbOut;
	unsigned char *bbLimit;
} machine_t;

/* modrm byte */
typedef union modrm_union modrm_union;
union modrm_union {
	struct {
		unsigned char rm : 3;
		unsigned char reg : 3;
		unsigned char mod : 2;
	} parts;  
	unsigned char byte;
};

typedef struct decode_s {
	unsigned long decode_eip;

	unsigned char *instr;
	unsigned char *pInstr;

	unsigned long b;
	unsigned opstate;

	modrm_union modrm;
	unsigned long modrm_regs;

	unsigned attr;

	unsigned char Group1_Prefix;
	unsigned char Group2_Prefix;
	unsigned char Group3_Prefix;
	unsigned char Group4_Prefix;
	unsigned char no_of_prefixes;
	unsigned flags;

	int (*emitfn)(unsigned long next_eip, struct decode_s *ds, 
			unsigned char* bbOut, unsigned char* bbLimit);
	void *pEntry;
} decode_t;

typedef int (*EMITFN)(unsigned long next_eip, struct decode_s *ds, 
                        unsigned char* bbOut, unsigned char* bbLimit);

typedef struct {
	MODETYPE amode;               /* addressing mode */
	MODETYPE ainfo;               /* mode-specific info */
} OpArgument;

#define OP_MAXARG 3
typedef struct {
	unsigned long index;          /* sanity check */

	char *disasm;                 /* dissassembly string */

	OpArgument args[OP_MAXARG];

	void *ptr;

	char *emitter_name;

	unsigned long attr;

	unsigned long flag_effect;    /* Whether the instruction sources/sinks/both the flags */
} OpCode; // __attribute__ ((aligned (32)));

extern const OpCode nopbyte0[256];
extern const OpCode twoByteOpcodes[256];

extern const OpCode group1_Eb_Ib[8];
extern const OpCode group1_Ev_Iv[8];
extern const OpCode group1_Ev_Ib[8];
extern const OpCode group2a_Eb_Ib[8];
extern const OpCode group2a_Ev_Ib[8];
extern const OpCode group2_Eb_1[8];
extern const OpCode group2_Ev_1[8];
extern const OpCode group2_Eb_CL[8];
extern const OpCode group2_Ev_CL[8];
extern const OpCode group3b[8];
extern const OpCode group3v[8];
extern const OpCode ngroup4[8];
extern const OpCode ngroup5[8];
extern const OpCode ngroup6[8];
extern const OpCode ngroup7[8];
extern const OpCode group8_Ev_Ib[8];
extern const OpCode ngroup9[8];

extern const OpCode float_d8[16];

extern const OpCode float_d9[16];
extern const OpCode float_d9_2[8];
extern const OpCode float_d9_4[8];
extern const OpCode float_d9_5[8];
extern const OpCode float_d9_6[8];
extern const OpCode float_d9_7[8];

extern const OpCode float_da[16];
extern const OpCode float_da_5[8];

extern const OpCode float_db[16];
extern const OpCode float_db_4[8];

extern const OpCode float_dc[16];

extern const OpCode float_dd[16];

extern const OpCode float_de[16];
extern const OpCode float_de_3[8];

extern const OpCode float_df[16];
extern const OpCode float_df_4[8];

#endif // _DEFS_H_
