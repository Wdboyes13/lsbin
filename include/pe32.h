#pragma once
#include <cstdint>

// sources: Microsoft PE/COFF Specification

#define DIRENT_IMP 1
#define NT_OPTHDR64_MAG 0x20b
#define NDIRENTS 16
#define SCTNAMSIZ 8 // section name size

// Offset to offset of PE header
const uint32_t PEHDROFFOFF = 0x3c;

typedef struct {
    uint32_t vaddr;
    uint32_t sz;
} ddir;

typedef struct {
    uint16_t machine;   // machine
    uint16_t numsects;  // number of sections
    uint32_t tstamp;    // timestamp
    uint32_t symtabptr; // symbol table ptr
    uint32_t nsyms;     // number of symbols
    uint16_t opthdrsz;  // optional header size
    uint16_t chars;     // charactersitics
} ntfilhdr;

typedef struct {
    uint16_t mag;         // magic
    uint8_t majlnkver;    // major link ver.
    uint8_t minlnkver;    // minor link ver.
    uint32_t codesz;      // code size
    uint32_t initdatasz;  // initialized data size
    uint32_t uinitdatasz; // uninitialized data size
    uint32_t entry;       // entry point
    uint32_t codebs;      // code base

    uint64_t imgbs; // image base

    uint32_t sectaln;     // section alignment
    uint32_t filaln;      // file alignment
    uint16_t majosver;    // major os ver.
    uint16_t minosver;    // minor os ver.
    uint16_t majimgver;   // major image ver.
    uint16_t minimgver;   // minor image ver.
    uint16_t majsubver;   // major subsystem ver.
    uint16_t minsubver;   // minor subsystem ver.
    uint32_t win32verval; // reserved
    uint32_t imgsz;       // size of image
    uint32_t hdrsz;       // size of headers
    uint32_t chksum;      // checksum
    uint16_t subsys;      // subsystem
    uint16_t dllchars;    // dll characteristics

    uint64_t stkrsrvsz; // stack reserve size
    uint64_t stkcomsz;  // stack commit size
    uint64_t hprsrvsz;  // heap reserve size
    uint64_t hpcomsz;   // heap commit size

    uint32_t ldrflags; // loader flags
    uint32_t nrva;     // number of rvas + sizes

    ddir datadirs[NDIRENTS];
} opthdr64;

typedef struct {
    uint16_t mag;         // magic
    uint8_t majlnkver;    // major link ver.
    uint8_t minlnkver;    // minor link ver.
    uint32_t codesz;      // code size
    uint32_t initdatasz;  // initialized data size
    uint32_t uinitdatasz; // uninitialized data size
    uint32_t entry;       // entry point
    uint32_t codebs;      // code base
    uint32_t databs;      // data base

    uint32_t imgbs; // image base

    uint32_t sectaln;     // section alignment
    uint32_t filaln;      // file alignment
    uint16_t majosver;    // major os ver.
    uint16_t minosver;    // minor os ver.
    uint16_t majimgver;   // major image ver.
    uint16_t minimgver;   // minor image ver.
    uint16_t majsubver;   // major subsystem ver.
    uint16_t minsubver;   // minor subsystem ver.
    uint32_t win32verval; // reserved
    uint32_t imgsz;       // size of image
    uint32_t hdrsz;       // size of headers
    uint32_t chksum;      // checksum
    uint16_t subsys;      // subsystem
    uint16_t dllchars;    // dll characteristics

    uint32_t stkrsrvsz; // stack reserve size
    uint32_t stkcomsz;  // stack commit size
    uint32_t hprsrvsz;  // heap reserve size
    uint32_t hpcomsz;   // heap commit size

    uint32_t ldrflags; // loader flags
    uint32_t nrva;     // number of rvas + sizes

    ddir datadirs[NDIRENTS];
} opthdr32;

typedef struct {
    uint32_t sig;
    ntfilhdr nthdr;
    opthdr64 opthdr;
} nthdrs64;

typedef struct {
    uint32_t sig;
    ntfilhdr nthdr;
    opthdr32 opthdr;
} nthdrs32;

typedef struct {
    uint32_t chars;     // characteristics
    uint32_t tstamp;    // timestamp
    uint32_t fwdchain;  // forwarder chain
    uint32_t namerva;   // name rva
    uint32_t thunkaddr; // rva to thunk table
} idesc;

typedef struct {
    uint8_t name[SCTNAMSIZ]; // name
    uint32_t virtsz;         // virtual size
    uint32_t vaddr;          // virtual address
    uint32_t rdatasz;        // size of raw data
    uint32_t rdataptr;       // pointer to raw data
    uint32_t rloptr;         // pointer to relocations
    uint32_t lnptr;          // pointer to line numbers
    uint16_t nrlo;           // number or relocations
    uint16_t nln;            // number of line numbers
    uint32_t chars;          // characteristics
} secthdr;

#define FOFF(tpy, fld) ((int32_t)__builtin_offsetof(tpy, fld))

#define FIRSTSECT(ntheader, ntheader_t)                                    \
    ((secthdr*)((uintptr_t)(ntheader) + FIELD_OFFSET(ntheader_t, opthdr) + \
                ((ntheader))->nthdr.opthdrsz))
