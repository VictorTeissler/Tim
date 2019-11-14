#ifndef TIM_WINSTRUCTS_H
#define TIM_WINSTRUCTS_H

#define _M_X64 1

namespace Tim {

typedef uint8_t UINT8;
typedef uint32_t ULONG32;
typedef int32_t LONG32;
typedef uint64_t UINT_PTR;
typedef uint64_t UINT64;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef void* PVOID;
typedef uint16_t* WCHAR;

const uint32_t PAGE_EXECUTE           = 0x10;
const uint32_t PAGE_EXECUTE_READ      = 0x20;
const uint32_t PAGE_EXECUTE_READWRITE = 0x40;
const uint32_t PAGE_EXECUTE_WRITECOPY = 0x80;
const uint32_t PAGE_NOACCESS          = 0x01;
const uint32_t PAGE_READONLY          = 0x02;
const uint32_t PAGE_READWRITE         = 0x04;
const uint32_t PAGE_WRITECOPY         = 0x08;
const uint32_t PAGE_GUARD             = 0x100;
const uint32_t PAGE_NOCACHE           = 0x200;
const uint32_t PAGE_WRITECOMBINE      = 0x400;


// And now for a few windows 10 structs tied to a very specific kernel version
// TODO: generalize structures using rekall profiles.. thats what they are for.
// NOTE: not done yet in the pursuit of time

#pragma pack(push, 1)

typedef struct _LIST_ENTRY                                                      // 2 / 2 elements; 0x0008 / 0x0010 Bytes
{
    _LIST_ENTRY*                 Flink;                                          // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    _LIST_ENTRY*                 Blink;                                          // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
} LIST_ENTRY, * PLIST_ENTRY;

typedef struct _RTL_BALANCED_NODE                                               // 6 / 6 elements; 0x000C / 0x0018 Bytes
{
    union                                                                       // 2 / 2 elements; 0x0008 / 0x0010 Bytes
    {
        _RTL_BALANCED_NODE*      Children[2];                                    // 0x0000 / 0x0000; 0x0008 / 0x0010 Bytes
        struct                                                                  // 2 / 2 elements; 0x0008 / 0x0010 Bytes
        {
            _RTL_BALANCED_NODE*  Left;                                           // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
            _RTL_BALANCED_NODE*  Right;                                          // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
        };
    };
    union                                                                       // 2 / 2 elements; 0x0004 / 0x0008 Bytes
    {
        struct                                                                  // 2 / 2 elements; 0x0001 / 0x0001 Bytes
        {
            UINT8               Red                                        : 1; // 0x0008 / 0x0010; Bit:   0
            UINT8               Balance                                    : 2; // 0x0008 / 0x0010; Bits:  0 -  1
        };
        UINT_PTR                ParentValue;                                    // 0x0008 / 0x0010; 0x0004 / 0x0008 Bytes
    };
} RTL_BALANCED_NODE, * PRTL_BALANCED_NODE;

typedef struct _RTL_AVL_TREE                                                    // 1 / 1 elements; 0x0004 / 0x0008 Bytes
{
        PRTL_BALANCED_NODE          Root;                                           // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
} RTL_AVL_TREE, * PRTL_AVL_TREE;

typedef struct _EX_PUSH_LOCK                                                    // 7 / 7 elements; 0x0004 / 0x0008 Bytes
{
    union                                                                       // 3 / 3 elements; 0x0004 / 0x0008 Bytes
    {
        struct                                                                  // 5 / 5 elements; 0x0004 / 0x0008 Bytes
        {
            UINT_PTR            Locked                                     : 1; // 0x0000 / 0x0000; Bit:   0
            UINT_PTR            Waiting                                    : 1; // 0x0000 / 0x0000; Bit:   1
            UINT_PTR            Waking                                     : 1; // 0x0000 / 0x0000; Bit:   2
            UINT_PTR            MultipleShared                             : 1; // 0x0000 / 0x0000; Bit:   3
#if defined(_M_X64)
            UINT64              Shared                                     :60; // ------ / 0x0000; Bits:  4 - 63
#else                                                                           // #if defined(_M_X64)
            ULONG32             Shared                                     :28; // 0x0000 / ------; Bits:  4 - 31
#endif                                                                          // #if defined(_M_X64)
        };
        UINT_PTR                Value;                                          // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
        PVOID                   Ptr;                                            // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    };
} EX_PUSH_LOCK, * PEX_PUSH_LOCK;

typedef struct _MMVAD_FLAGS                                                     // 16 / 16 elements; 0x0004 / 0x0004 Bytes
{
    ULONG32                     VadType                                    : 3; // 0x0000 / 0x0000; Bits:  0 -  2
    ULONG32                     Protection                                 : 5; // 0x0000 / 0x0000; Bits:  3 -  7
    ULONG32                     PreferredNode                              : 6; // 0x0000 / 0x0000; Bits:  8 - 13
    ULONG32                     PrivateMemory                              : 1; // 0x0000 / 0x0000; Bit:  14
    ULONG32                     PrivateFixup                               : 1; // 0x0000 / 0x0000; Bit:  15
    ULONG32                     Graphics                                   : 1; // 0x0000 / 0x0000; Bit:  16
    ULONG32                     Enclave                                    : 1; // 0x0000 / 0x0000; Bit:  17
    ULONG32                     PageSize64K                                : 1; // 0x0000 / 0x0000; Bit:  18
    ULONG32                     ShadowStack                                : 1; // 0x0000 / 0x0000; Bit:  19
    ULONG32                     Spare                                      : 6; // 0x0000 / 0x0000; Bits: 20 - 25
    ULONG32                     HotPatchAllowed                            : 1; // 0x0000 / 0x0000; Bit:  26
    ULONG32                     NoChange                                   : 1; // 0x0000 / 0x0000; Bit:  27
    ULONG32                     ManySubsections                            : 1; // 0x0000 / 0x0000; Bit:  28
    ULONG32                     DeleteInProgress                           : 1; // 0x0000 / 0x0000; Bit:  29
    ULONG32                     LockContended                              : 1; // 0x0000 / 0x0000; Bit:  30
    ULONG32                     Lock                                       : 1; // 0x0000 / 0x0000; Bit:  31
} MMVAD_FLAGS, * PMMVAD_FLAGS;

typedef struct _MMVAD_FLAGS1                                                    // 2 / 2 elements; 0x0004 / 0x0004 Bytes
{
    ULONG32                     CommitCharge                               :31; // 0x0000 / 0x0000; Bits:  0 - 30
    ULONG32                     MemCommit                                  : 1; // 0x0000 / 0x0000; Bit:  31
} MMVAD_FLAGS1, * PMMVAD_FLAGS1;

typedef struct _MMVAD_FLAGS2                                                    // 7 / 7 elements; 0x0004 / 0x0004 Bytes
{
    ULONG32                     FileOffset                                 :24; // 0x0000 / 0x0000; Bits:  0 - 23
    ULONG32                     Large                                      : 1; // 0x0000 / 0x0000; Bit:  24
    ULONG32                     TrimBehind                                 : 1; // 0x0000 / 0x0000; Bit:  25
    ULONG32                     Inherit                                    : 1; // 0x0000 / 0x0000; Bit:  26
    ULONG32                     NoValidationNeeded                         : 1; // 0x0000 / 0x0000; Bit:  27
    ULONG32                     PrivateDemandZero                          : 1; // 0x0000 / 0x0000; Bit:  28
    ULONG32                     Spare                                      : 3; // 0x0000 / 0x0000; Bits: 29 - 31
} MMVAD_FLAGS2, * PMMVAD_FLAGS2;

typedef struct _MMVAD_SHORT                                                     // 10 / 14 elements; 0x0028 / 0x0040 Bytes
{
    union                                                                       // 2 / 2 elements; 0x000C / 0x0018 Bytes
    {
        struct                                                                  // 2 / 2 elements; 0x0008 / 0x0010 Bytes
        {
            _MMVAD_SHORT*       NextVad;                                        // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
            PVOID               ExtraCreateInfo;                                // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
        };
        RTL_BALANCED_NODE       VadNode;                                        // 0x0000 / 0x0000; 0x000C / 0x0018 Bytes
    };
    ULONG32                     StartingVpn;                                    // 0x000C / 0x0018; 0x0004 / 0x0004 Bytes
    ULONG32                     EndingVpn;                                      // 0x0010 / 0x001C; 0x0004 / 0x0004 Bytes
#if defined(_M_X64)
    UINT8                       StartingVpnHigh;                                // ------ / 0x0020; ------ / 0x0001 Bytes
    UINT8                       EndingVpnHigh;                                  // ------ / 0x0021; ------ / 0x0001 Bytes
    UINT8                       CommitChargeHigh;                               // ------ / 0x0022; ------ / 0x0001 Bytes
    UINT8                       SpareNT64VadUChar;                              // ------ / 0x0023; ------ / 0x0001 Bytes
#endif                                                                          // #if defined(_M_X64)
    LONG32                      ReferenceCount;                                 // 0x0014 / 0x0024; 0x0004 / 0x0004 Bytes
    EX_PUSH_LOCK                PushLock;                                       // 0x0018 / 0x0028; 0x0004 / 0x0008 Bytes
    union                                                                       // 0x001C / 0x0030; 3 / 3 elements; 0x0004 / 0x0004 Bytes
    {
        ULONG32                 LongFlags;                                      // 0x001C / 0x0030; 0x0004 / 0x0004 Bytes
        MMVAD_FLAGS VadFlags;                                                   // 0x001C / 0x0030; 0x0004 / 0x0004 Bytes
        ULONG32                 VolatileVadLong;                                // 0x001C / 0x0030; 0x0004 / 0x0004 Bytes
    } u;                                                                        // 0x001C / 0x0030; 0x0004 / 0x0004 Bytes
    union                                                                       // 0x0020 / 0x0034; 2 / 2 elements; 0x0004 / 0x0004 Bytes
    {
        ULONG32                 LongFlags1;                                     // 0x0020 / 0x0034; 0x0004 / 0x0004 Bytes
        MMVAD_FLAGS1 VadFlags1;                                                 // 0x0020 / 0x0034; 0x0004 / 0x0004 Bytes
    } u1;                                                                       // 0x0020 / 0x0034; 0x0004 / 0x0004 Bytes
    void* /* hack to avoid extra types */         EventList;                                      // 0x0024 / 0x0038; 0x0004 / 0x0008 Bytes
} MMVAD_SHORT, * PMMVAD_SHORT;

typedef struct _MMSUBSECTION_FLAGS                                              // 8 / 8 elements; 0x0004 / 0x0004 Bytes
{
    UINT16                      SubsectionAccessed                         : 1; // 0x0000 / 0x0000; Bit:   0
    UINT16                      Protection                                 : 5; // 0x0000 / 0x0000; Bits:  1 -  5
    UINT16                      StartingSector4132                         :10; // 0x0000 / 0x0000; Bits:  6 - 15
    UINT16                      SubsectionStatic                           : 1; // 0x0002 / 0x0002; Bit:   0
    UINT16                      GlobalMemory                               : 1; // 0x0002 / 0x0002; Bit:   1
    UINT16                      Spare                                      : 1; // 0x0002 / 0x0002; Bit:   2
    UINT16                      OnDereferenceList                          : 1; // 0x0002 / 0x0002; Bit:   3
    UINT16                      SectorEndOffset                            :12; // 0x0002 / 0x0002; Bits:  4 - 15
} MMSUBSECTION_FLAGS, * PMMSUBSECTION_FLAGS;

typedef struct _MI_SUBSECTION_ENTRY1                                            // 2 / 2 elements; 0x0004 / 0x0004 Bytes
{
    ULONG32                     CrossPartitionReferences                   :30; // 0x0000 / 0x0000; Bits:  0 - 29
    ULONG32                     SubsectionMappedLarge                      : 2; // 0x0000 / 0x0000; Bits: 30 - 31
} MI_SUBSECTION_ENTRY1, * PMI_SUBSECTION_ENTRY1;

typedef struct _MMPTE_HIGHLOW                                                   // 2 / - elements; 0x0008 / ------ Bytes
{
    ULONG32                     LowPart;                                        // 0x0000 / ------; 0x0004 / ------ Bytes
    ULONG32                     HighPart;                                       // 0x0004 / ------; 0x0004 / ------ Bytes
} MMPTE_HIGHLOW, * PMMPTE_HIGHLOW;

typedef struct _MMPTE_HARDWARE                                                  // 15 / 18 elements; 0x0008 / 0x0008 Bytes
{
    UINT64                      Valid                                      : 1; // 0x0000 / 0x0000; Bit:   0
    UINT64                      Dirty1                                     : 1; // 0x0000 / 0x0000; Bit:   1
    UINT64                      Owner                                      : 1; // 0x0000 / 0x0000; Bit:   2
    UINT64                      WriteThrough                               : 1; // 0x0000 / 0x0000; Bit:   3
    UINT64                      CacheDisable                               : 1; // 0x0000 / 0x0000; Bit:   4
    UINT64                      Accessed                                   : 1; // 0x0000 / 0x0000; Bit:   5
    UINT64                      Dirty                                      : 1; // 0x0000 / 0x0000; Bit:   6
    UINT64                      LargePage                                  : 1; // 0x0000 / 0x0000; Bit:   7
    UINT64                      Global                                     : 1; // 0x0000 / 0x0000; Bit:   8
    UINT64                      CopyOnWrite                                : 1; // 0x0000 / 0x0000; Bit:   9
    UINT64                      Unused                                     : 1; // 0x0000 / 0x0000; Bit:  10
    UINT64                      Write                                      : 1; // 0x0000 / 0x0000; Bit:  11
#if defined(_M_X64)
    UINT64                      PageFrameNumber                            :36; // ------ / 0x0000; Bits: 12 - 47
#else                                                                           // #if defined(_M_X64)
    UINT64                      PageFrameNumber                            :26; // 0x0000 / ------; Bits: 12 - 37
#endif                                                                          // #if defined(_M_X64)
#if defined(_M_X64)
    UINT64                      ReservedForHardware                        : 4; // ------ / 0x0000; Bits: 48 - 51
    UINT64                      ReservedForSoftware                        : 4; // ------ / 0x0000; Bits: 52 - 55
    UINT64                      WsleAge                                    : 4; // ------ / 0x0000; Bits: 56 - 59
    UINT64                      WsleProtection                             : 3; // ------ / 0x0000; Bits: 60 - 62
#else                                                                           // #if defined(_M_X64)
    UINT64                      reserved1                                  :25; // 0x0000 / ------; Bits: 38 - 62
#endif                                                                          // #if defined(_M_X64)
    UINT64                      NoExecute                                  : 1; // 0x0000 / 0x0000; Bit:  63
} MMPTE_HARDWARE, * PMMPTE_HARDWARE;

typedef struct _MMPTE_PROTOTYPE                                                 // 10 / 10 elements; 0x0008 / 0x0008 Bytes
{
    UINT64                      Valid                                      : 1; // 0x0000 / 0x0000; Bit:   0
    UINT64                      DemandFillProto                            : 1; // 0x0000 / 0x0000; Bit:   1
    UINT64                      HiberVerifyConverted                       : 1; // 0x0000 / 0x0000; Bit:   2
    UINT64                      ReadOnly                                   : 1; // 0x0000 / 0x0000; Bit:   3
    UINT64                      SwizzleBit                                 : 1; // 0x0000 / 0x0000; Bit:   4
    UINT64                      Protection                                 : 5; // 0x0000 / 0x0000; Bits:  5 -  9
    UINT64                      Prototype                                  : 1; // 0x0000 / 0x0000; Bit:  10
    UINT64                      Combined                                   : 1; // 0x0000 / 0x0000; Bit:  11
#if defined(_M_X64)
    UINT64                      Unused1                                    : 4; // ------ / 0x0000; Bits: 12 - 15
#else                                                                           // #if defined(_M_X64)
    UINT64                      Unused                                     :20; // 0x0000 / ------; Bits: 12 - 31
#endif                                                                          // #if defined(_M_X64)
#if defined(_M_X64)
    INT64                       ProtoAddress                               :48; // ------ / 0x0000; Bits: 16 - 63
#else                                                                           // #if defined(_M_X64)
    UINT64                      ProtoAddress                               :32; // 0x0000 / ------; Bits: 32 - 63
#endif                                                                          // #if defined(_M_X64)
} MMPTE_PROTOTYPE, * PMMPTE_PROTOTYPE;

typedef struct _MMPTE_SOFTWARE                                                  // 11 / 13 elements; 0x0008 / 0x0008 Bytes
{
    UINT64                      Valid                                      : 1; // 0x0000 / 0x0000; Bit:   0
    UINT64                      PageFileReserved                           : 1; // 0x0000 / 0x0000; Bit:   1
    UINT64                      PageFileAllocated                          : 1; // 0x0000 / 0x0000; Bit:   2
#if defined(_M_X64)
    UINT64                      ColdPage                                   : 1; // ------ / 0x0000; Bit:   3
#else                                                                           // #if defined(_M_X64)
    UINT64                      Unused0                                    : 1; // 0x0000 / ------; Bit:   3
#endif                                                                          // #if defined(_M_X64)
    UINT64                      SwizzleBit                                 : 1; // 0x0000 / 0x0000; Bit:   4
    UINT64                      Protection                                 : 5; // 0x0000 / 0x0000; Bits:  5 -  9
    UINT64                      Prototype                                  : 1; // 0x0000 / 0x0000; Bit:  10
    UINT64                      Transition                                 : 1; // 0x0000 / 0x0000; Bit:  11
    UINT64                      PageFileLow                                : 4; // 0x0000 / 0x0000; Bits: 12 - 15
#if defined(_M_X64)
    UINT64                      UsedPageTableEntries                       :10; // ------ / 0x0000; Bits: 16 - 25
    UINT64                      ShadowStack                                : 1; // ------ / 0x0000; Bit:  26
    UINT64                      Unused                                     : 5; // ------ / 0x0000; Bits: 27 - 31
#else                                                                           // #if defined(_M_X64)
    UINT64                      Unused1                                    :16; // 0x0000 / ------; Bits: 16 - 31
#endif                                                                          // #if defined(_M_X64)
    UINT64                      PageFileHigh                               :32; // 0x0000 / 0x0000; Bits: 32 - 63
} MMPTE_SOFTWARE, * PMMPTE_SOFTWARE;

typedef struct _MMPTE_TIMESTAMP                                                 // 9 / 9 elements; 0x0008 / 0x0008 Bytes
{
    UINT64                      MustBeZero                                 : 1; // 0x0000 / 0x0000; Bit:   0
#if defined(_M_X64)
    UINT64                      Unused                                     : 3; // ------ / 0x0000; Bits:  1 -  3
#else                                                                           // #if defined(_M_X64)
    UINT64                      Unused0                                    : 3; // 0x0000 / ------; Bits:  1 -  3
#endif                                                                          // #if defined(_M_X64)
    UINT64                      SwizzleBit                                 : 1; // 0x0000 / 0x0000; Bit:   4
    UINT64                      Protection                                 : 5; // 0x0000 / 0x0000; Bits:  5 -  9
    UINT64                      Prototype                                  : 1; // 0x0000 / 0x0000; Bit:  10
    UINT64                      Transition                                 : 1; // 0x0000 / 0x0000; Bit:  11
    UINT64                      PageFileLow                                : 4; // 0x0000 / 0x0000; Bits: 12 - 15
#if defined(_M_X64)
    UINT64                      Reserved                                   :16; // ------ / 0x0000; Bits: 16 - 31
#else                                                                           // #if defined(_M_X64)
    UINT64                      Unused1                                    :16; // 0x0000 / ------; Bits: 16 - 31
#endif                                                                          // #if defined(_M_X64)
    UINT64                      GlobalTimeStamp                            :32; // 0x0000 / 0x0000; Bits: 32 - 63
} MMPTE_TIMESTAMP, * PMMPTE_TIMESTAMP;

typedef struct _MMPTE_TRANSITION                                                // 10 / 10 elements; 0x0008 / 0x0008 Bytes
{
    UINT64                      Valid                                      : 1; // 0x0000 / 0x0000; Bit:   0
    UINT64                      Write                                      : 1; // 0x0000 / 0x0000; Bit:   1
    UINT64                      Spare                                      : 1; // 0x0000 / 0x0000; Bit:   2
    UINT64                      IoTracker                                  : 1; // 0x0000 / 0x0000; Bit:   3
    UINT64                      SwizzleBit                                 : 1; // 0x0000 / 0x0000; Bit:   4
    UINT64                      Protection                                 : 5; // 0x0000 / 0x0000; Bits:  5 -  9
    UINT64                      Prototype                                  : 1; // 0x0000 / 0x0000; Bit:  10
    UINT64                      Transition                                 : 1; // 0x0000 / 0x0000; Bit:  11
#if defined(_M_X64)
    UINT64                      PageFrameNumber                            :36; // ------ / 0x0000; Bits: 12 - 47
#else                                                                           // #if defined(_M_X64)
    UINT64                      PageFrameNumber                            :26; // 0x0000 / ------; Bits: 12 - 37
#endif                                                                          // #if defined(_M_X64)
#if defined(_M_X64)
    UINT64                      Unused                                     :16; // ------ / 0x0000; Bits: 48 - 63
#else                                                                           // #if defined(_M_X64)
    UINT64                      Unused                                     :26; // 0x0000 / ------; Bits: 38 - 63
#endif                                                                          // #if defined(_M_X64)
} MMPTE_TRANSITION, * PMMPTE_TRANSITION;

typedef struct _MMPTE_SUBSECTION                                                // 7 / 9 elements; 0x0008 / 0x0008 Bytes
{
    UINT64                      Valid                                      : 1; // 0x0000 / 0x0000; Bit:   0
    UINT64                      Unused0                                    : 3; // 0x0000 / 0x0000; Bits:  1 -  3
    UINT64                      SwizzleBit                                 : 1; // 0x0000 / 0x0000; Bit:   4
    UINT64                      Protection                                 : 5; // 0x0000 / 0x0000; Bits:  5 -  9
    UINT64                      Prototype                                  : 1; // 0x0000 / 0x0000; Bit:  10
#if defined(_M_X64)
    UINT64                      ColdPage                                   : 1; // ------ / 0x0000; Bit:  11
#endif                                                                          // #if defined(_M_X64)
#if defined(_M_X64)
    UINT64                      Unused1                                    : 3; // ------ / 0x0000; Bits: 12 - 14
#else                                                                           // #if defined(_M_X64)
    UINT64                      Unused1                                    :21; // 0x0000 / ------; Bits: 11 - 31
#endif                                                                          // #if defined(_M_X64)
#if defined(_M_X64)
    UINT64                      ExecutePrivilege                           : 1; // ------ / 0x0000; Bit:  15
#endif                                                                          // #if defined(_M_X64)
#if defined(_M_X64)
    INT64                       SubsectionAddress                          :48; // ------ / 0x0000; Bits: 16 - 63
#else                                                                           // #if defined(_M_X64)
    UINT64                      SubsectionAddress                          :32; // 0x0000 / ------; Bits: 32 - 63
#endif                                                                          // #if defined(_M_X64)
} MMPTE_SUBSECTION, * PMMPTE_SUBSECTION;

typedef struct _MMPTE_LIST                                                      // 9 / 9 elements; 0x0008 / 0x0008 Bytes
{
    UINT64                      Valid                                      : 1; // 0x0000 / 0x0000; Bit:   0
    UINT64                      OneEntry                                   : 1; // 0x0000 / 0x0000; Bit:   1
    UINT64                      filler0                                    : 2; // 0x0000 / 0x0000; Bits:  2 -  3
    UINT64                      SwizzleBit                                 : 1; // 0x0000 / 0x0000; Bit:   4
#if defined(_M_X64)
    UINT64                      Protection                                 : 5; // ------ / 0x0000; Bits:  5 -  9
#else                                                                           // #if defined(_M_X64)
    UINT64                      filler1                                    : 5; // 0x0000 / ------; Bits:  5 -  9
#endif                                                                          // #if defined(_M_X64)
    UINT64                      Prototype                                  : 1; // 0x0000 / 0x0000; Bit:  10
    UINT64                      Transition                                 : 1; // 0x0000 / 0x0000; Bit:  11
#if defined(_M_X64)
    UINT64                      filler1                                    :16; // ------ / 0x0000; Bits: 12 - 27
#else                                                                           // #if defined(_M_X64)
    UINT64                      filler2                                    :20; // 0x0000 / ------; Bits: 12 - 31
#endif                                                                          // #if defined(_M_X64)
#if defined(_M_X64)
    UINT64                      NextEntry                                  :36; // ------ / 0x0000; Bits: 28 - 63
#else                                                                           // #if defined(_M_X64)
    UINT64                      NextEntry                                  :32; // 0x0000 / ------; Bits: 32 - 63
#endif                                                                          // #if defined(_M_X64)
} MMPTE_LIST, * PMMPTE_LIST;


typedef struct _MMPTE                                                           // 1 / 1 elements; 0x0008 / 0x0008 Bytes
{
    union                                                                       // 10 /  9 elements; 0x0008 / 0x0008 Bytes
    {
        UINT64                  Long;                                           // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
        UINT64                  VolatileLong;                                   // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
#if !defined(_M_X64)
        MMPTE_HIGHLOW HighLow;                                                  // 0x0000 / ------; 0x0008 / ------ Bytes
#endif                                                                          // #if !defined(_M_X64)
        MMPTE_HARDWARE          Hard;                                           // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
        MMPTE_PROTOTYPE         Proto;                                          // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
        MMPTE_SOFTWARE          Soft;                                           // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
        MMPTE_TIMESTAMP         TimeStamp;                                      // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
        MMPTE_TRANSITION        Trans;                                          // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
        MMPTE_SUBSECTION        Subsect;                                        // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
        MMPTE_LIST              List;                                           // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
    }u;                                                                         // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
} MMPTE, * PMMPTE;


typedef struct _EX_FAST_REF                                                     // 3 / 3 elements; 0x0004 / 0x0008 Bytes
{
    union                                                                       // 3 / 3 elements; 0x0004 / 0x0008 Bytes
    {
        PVOID                   Object;                                         // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
#if defined(_M_X64)
        UINT64                  RefCnt                                     : 4; // ------ / 0x0000; Bits:  0 -  3
#else                                                                           // #if defined(_M_X64)
        ULONG32                 RefCnt                                     : 3; // 0x0000 / ------; Bits:  0 -  2
#endif                                                                          // #if defined(_M_X64)
        UINT_PTR                Value;                                          // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    };
} EX_FAST_REF, * PEX_FAST_REF;

typedef struct _MMSECTION_FLAGS                                                 // 27 / 27 elements; 0x0004 / 0x0004 Bytes
{
    UINT32                      BeingDeleted                               : 1; // 0x0000 / 0x0000; Bit:   0
    UINT32                      BeingCreated                               : 1; // 0x0000 / 0x0000; Bit:   1
    UINT32                      BeingPurged                                : 1; // 0x0000 / 0x0000; Bit:   2
    UINT32                      NoModifiedWriting                          : 1; // 0x0000 / 0x0000; Bit:   3
    UINT32                      FailAllIo                                  : 1; // 0x0000 / 0x0000; Bit:   4
    UINT32                      Image                                      : 1; // 0x0000 / 0x0000; Bit:   5
    UINT32                      Based                                      : 1; // 0x0000 / 0x0000; Bit:   6
    UINT32                      File                                       : 1; // 0x0000 / 0x0000; Bit:   7
    UINT32                      AttemptingDelete                           : 1; // 0x0000 / 0x0000; Bit:   8
    UINT32                      PrefetchCreated                            : 1; // 0x0000 / 0x0000; Bit:   9
    UINT32                      PhysicalMemory                             : 1; // 0x0000 / 0x0000; Bit:  10
    UINT32                      ImageControlAreaOnRemovableMedia           : 1; // 0x0000 / 0x0000; Bit:  11
    UINT32                      Reserve                                    : 1; // 0x0000 / 0x0000; Bit:  12
    UINT32                      Commit                                     : 1; // 0x0000 / 0x0000; Bit:  13
    UINT32                      NoChange                                   : 1; // 0x0000 / 0x0000; Bit:  14
    UINT32                      WasPurged                                  : 1; // 0x0000 / 0x0000; Bit:  15
    UINT32                      UserReference                              : 1; // 0x0000 / 0x0000; Bit:  16
    UINT32                      GlobalMemory                               : 1; // 0x0000 / 0x0000; Bit:  17
    UINT32                      DeleteOnClose                              : 1; // 0x0000 / 0x0000; Bit:  18
    UINT32                      FilePointerNull                            : 1; // 0x0000 / 0x0000; Bit:  19
    UINT32                      PreferredNode                              : 6; // 0x0000 / 0x0000; Bits: 20 - 25
    UINT32                      GlobalOnlyPerSession                       : 1; // 0x0000 / 0x0000; Bit:  26
    UINT32                      UserWritable                               : 1; // 0x0000 / 0x0000; Bit:  27
    UINT32                      SystemVaAllocated                          : 1; // 0x0000 / 0x0000; Bit:  28
    UINT32                      PreferredFsCompressionBoundary             : 1; // 0x0000 / 0x0000; Bit:  29
    UINT32                      UsingFileExtents                           : 1; // 0x0000 / 0x0000; Bit:  30
    UINT32                      PageSize64K                                : 1; // 0x0000 / 0x0000; Bit:  31
} MMSECTION_FLAGS, * PMMSECTION_FLAGS;

typedef struct _MMSECTION_FLAGS2                                                // - / 3 elements; ------ / 0x0004 Bytes
{
    UINT16                      PartitionId                                :10; // ------ / 0x0000; Bits:  0 -  9
    UINT8                       NoCrossPartitionAccess                     : 1; // ------ / 0x0002; Bit:   0
    UINT8                       SubsectionCrossPartitionReferenceOverflow  : 1; // ------ / 0x0002; Bit:   1
} MMSECTION_FLAGS2, * PMMSECTION_FLAGS2;

struct _CONTROL_AREA;

typedef struct _SEGMENT_FLAGS                                                   // 16 / 16 elements; 0x0004 / 0x0004 Bytes
{
    union                                                                       // 2 / 2 elements; 0x0002 / 0x0002 Bytes
    {
        struct                                                                  // 6 / 6 elements; 0x0002 / 0x0002 Bytes
        {
            UINT16              TotalNumberOfPtes4132                      :10; // 0x0000 / 0x0000; Bits:  0 -  9
            UINT16              Spare0                                     : 2; // 0x0000 / 0x0000; Bits: 10 - 11
            UINT16              LargePages                                 : 1; // 0x0000 / 0x0000; Bit:  12
            UINT16              DebugSymbolsLoaded                         : 1; // 0x0000 / 0x0000; Bit:  13
            UINT16              WriteCombined                              : 1; // 0x0000 / 0x0000; Bit:  14
            UINT16              NoCache                                    : 1; // 0x0000 / 0x0000; Bit:  15
        };
        UINT16                  Short0;                                         // 0x0000 / 0x0000; 0x0002 / 0x0002 Bytes
    };
    union                                                                       // 2 / 2 elements; 0x0001 / 0x0001 Bytes
    {
        struct                                                                  // 4 / 4 elements; 0x0001 / 0x0001 Bytes
        {
            UINT8               Unused                                     : 1; // 0x0002 / 0x0002; Bit:   0
            UINT8               DefaultProtectionMask                      : 5; // 0x0002 / 0x0002; Bits:  1 -  5
            UINT8               Binary32                                   : 1; // 0x0002 / 0x0002; Bit:   6
            UINT8               ContainsDebug                              : 1; // 0x0002 / 0x0002; Bit:   7
        };
        UINT8                   UChar1;                                         // 0x0002 / 0x0002; 0x0001 / 0x0001 Bytes
    };
    union                                                                       // 2 / 2 elements; 0x0001 / 0x0001 Bytes
    {
        struct                                                                  // 3 / 3 elements; 0x0001 / 0x0001 Bytes
        {
            UINT8               ForceCollision                             : 1; // 0x0003 / 0x0003; Bit:   0
            UINT8               ImageSigningType                           : 3; // 0x0003 / 0x0003; Bits:  1 -  3
            UINT8               ImageSigningLevel                          : 4; // 0x0003 / 0x0003; Bits:  4 -  7
        };
        UINT8                   UChar2;                                         // 0x0003 / 0x0003; 0x0001 / 0x0001 Bytes
    };
} SEGMENT_FLAGS, * PSEGMENT_FLAGS;

typedef struct _MMEXTEND_INFO                                                   // 3 / 3 elements; 0x0010 / 0x0010 Bytes
{
    UINT64                      CommittedSize;                                  // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
    ULONG32                     ReferenceCount;                                 // 0x0008 / 0x0008; 0x0004 / 0x0004 Bytes
    UINT8                       _PADDING0_[4];                                  // 0x000C / 0x000C; 0x0004 / 0x0004 Bytes
} MMEXTEND_INFO, * PMMEXTEND_INFO;

typedef struct _SECTION_IMAGE_INFORMATION                                       // 26 / 27 elements; 0x0030 / 0x0040 Bytes
{
    PVOID                       TransferAddress;                                // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    ULONG32                     ZeroBits;                                       // 0x0004 / 0x0008; 0x0004 / 0x0004 Bytes
#if defined(_M_X64)
    UINT8                       _PADDING0_[4];                                  // ------ / 0x000C; ------ / 0x0004 Bytes
#endif                                                                          // #if defined(_M_X64)
    UINT_PTR                    MaximumStackSize;                               // 0x0008 / 0x0010; 0x0004 / 0x0008 Bytes
    UINT_PTR                    CommittedStackSize;                             // 0x000C / 0x0018; 0x0004 / 0x0008 Bytes
    ULONG32                     SubSystemType;                                  // 0x0010 / 0x0020; 0x0004 / 0x0004 Bytes
    union                                                                       // 2 / 2 elements; 0x0004 / 0x0004 Bytes
    {
        struct                                                                  // 2 / 2 elements; 0x0004 / 0x0004 Bytes
        {
            UINT16              SubSystemMinorVersion;                          // 0x0014 / 0x0024; 0x0002 / 0x0002 Bytes
            UINT16              SubSystemMajorVersion;                          // 0x0016 / 0x0026; 0x0002 / 0x0002 Bytes
        };
        ULONG32                 SubSystemVersion;                               // 0x0014 / 0x0024; 0x0004 / 0x0004 Bytes
    };
    union                                                                       // 2 / 2 elements; 0x0004 / 0x0004 Bytes
    {
        struct                                                                  // 2 / 2 elements; 0x0004 / 0x0004 Bytes
        {
            UINT16              MajorOperatingSystemVersion;                    // 0x0018 / 0x0028; 0x0002 / 0x0002 Bytes
            UINT16              MinorOperatingSystemVersion;                    // 0x001A / 0x002A; 0x0002 / 0x0002 Bytes
        };
        ULONG32                 OperatingSystemVersion;                         // 0x0018 / 0x0028; 0x0004 / 0x0004 Bytes
    };
    UINT16                      ImageCharacteristics;                           // 0x001C / 0x002C; 0x0002 / 0x0002 Bytes
    UINT16                      DllCharacteristics;                             // 0x001E / 0x002E; 0x0002 / 0x0002 Bytes
    UINT16                      Machine;                                        // 0x0020 / 0x0030; 0x0002 / 0x0002 Bytes
    UINT8                       ImageContainsCode;                              // 0x0022 / 0x0032; 0x0001 / 0x0001 Bytes
    union                                                                       // 2 / 2 elements; 0x0001 / 0x0001 Bytes
    {
        UINT8                   ImageFlags;                                     // 0x0023 / 0x0033; 0x0001 / 0x0001 Bytes
        struct                                                                  // 7 / 7 elements; 0x0001 / 0x0001 Bytes
        {
            UINT8               ComPlusNativeReady                         : 1; // 0x0023 / 0x0033; Bit:   0
            UINT8               ComPlusILOnly                              : 1; // 0x0023 / 0x0033; Bit:   1
            UINT8               ImageDynamicallyRelocated                  : 1; // 0x0023 / 0x0033; Bit:   2
            UINT8               ImageMappedFlat                            : 1; // 0x0023 / 0x0033; Bit:   3
            UINT8               BaseBelow4gb                               : 1; // 0x0023 / 0x0033; Bit:   4
            UINT8               ComPlusPrefer32bit                         : 1; // 0x0023 / 0x0033; Bit:   5
            UINT8               Reserved                                   : 2; // 0x0023 / 0x0033; Bits:  6 -  7
        };
    };
    ULONG32                     LoaderFlags;                                    // 0x0024 / 0x0034; 0x0004 / 0x0004 Bytes
    ULONG32                     ImageFileSize;                                  // 0x0028 / 0x0038; 0x0004 / 0x0004 Bytes
    ULONG32                     CheckSum;                                       // 0x002C / 0x003C; 0x0004 / 0x0004 Bytes
} SECTION_IMAGE_INFORMATION, * PSECTION_IMAGE_INFORMATION;

typedef struct _MI_EXTRA_IMAGE_INFORMATION                                      // 3 / 3 elements; 0x000C / 0x000C Bytes
{
    ULONG32                     SizeOfHeaders;                                  // 0x0000 / 0x0000; 0x0004 / 0x0004 Bytes
    ULONG32                     SizeOfImage;                                    // 0x0004 / 0x0004; 0x0004 / 0x0004 Bytes
    ULONG32                     TimeDateStamp;                                  // 0x0008 / 0x0008; 0x0004 / 0x0004 Bytes
} MI_EXTRA_IMAGE_INFORMATION, * PMI_EXTRA_IMAGE_INFORMATION;

typedef struct _MI_SECTION_IMAGE_INFORMATION                                    // 2 / 3 elements; 0x003C / 0x0050 Bytes
{
        SECTION_IMAGE_INFORMATION   ExportedImageInformation;                       // 0x0000 / 0x0000; 0x0030 / 0x0040 Bytes
            MI_EXTRA_IMAGE_INFORMATION  InternalImageInformation;                       // 0x0030 / 0x0040; 0x000C / 0x000C Bytes
#if defined(_M_X64)
                UINT8                       _PADDING0_[4];                                  // ------ / 0x004C; ------ / 0x0004 Bytes
#endif                                                                          // #if defined(_M_X64)
} MI_SECTION_IMAGE_INFORMATION, * PMI_SECTION_IMAGE_INFORMATION;

typedef struct _SEGMENT                                                         // 12 / 11 elements; 0x0030 / 0x0048 Bytes
{
    _CONTROL_AREA*              ControlArea;                                    // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    ULONG32                     TotalNumberOfPtes;                              // 0x0004 / 0x0008; 0x0004 / 0x0004 Bytes
    SEGMENT_FLAGS               SegmentFlags;                                   // 0x0008 / 0x000C; 0x0004 / 0x0004 Bytes
    UINT_PTR                    NumberOfCommittedPages;                         // 0x000C / 0x0010; 0x0004 / 0x0008 Bytes
    UINT64                      SizeOfSegment;                                  // 0x0010 / 0x0018; 0x0008 / 0x0008 Bytes
    union                                                                       // 2 / 2 elements; 0x0004 / 0x0008 Bytes
    {
        PMMEXTEND_INFO          ExtendInfo;                                     // 0x0018 / 0x0020; 0x0004 / 0x0008 Bytes
        PVOID                   BasedAddress;                                   // 0x0018 / 0x0020; 0x0004 / 0x0008 Bytes
    };
    EX_PUSH_LOCK                SegmentLock;                                    // 0x001C / 0x0028; 0x0004 / 0x0008 Bytes
    union                                                                       // 0x0020 / 0x0030; 2 / 2 elements; 0x0004 / 0x0008 Bytes
    {
        UINT_PTR                ImageCommitment;                                // 0x0020 / 0x0030; 0x0004 / 0x0008 Bytes
        ULONG32                 CreatingProcessId;                              // 0x0020 / 0x0030; 0x0004 / 0x0004 Bytes
    } u1;                                                                       // 0x0020 / 0x0030; 0x0004 / 0x0008 Bytes
    union                                                                       // 0x0024 / 0x0038; 2 / 2 elements; 0x0004 / 0x0008 Bytes
    {
        PMI_SECTION_IMAGE_INFORMATION 
                                ImageInformation;                               // 0x0024 / 0x0038; 0x0004 / 0x0008 Bytes
        PVOID                   FirstMappedVa;                                  // 0x0024 / 0x0038; 0x0004 / 0x0008 Bytes
    } u2;                                                                       // 0x0024 / 0x0038; 0x0004 / 0x0008 Bytes
    PMMPTE                      PrototypePte;                                   // 0x0028 / 0x0040; 0x0004 / 0x0008 Bytes
#if !defined(_M_X64)
    UINT8                       _PADDING0_[4];                                  // 0x002C / ------; 0x0004 / ------ Bytes
#endif                                                                          // #if !defined(_M_X64)
} SEGMENT, * PSEGMENT;


typedef struct _CONTROL_AREA                                                    // 17 / 17 elements; 0x0050 / 0x0080 Bytes
{
    PSEGMENT                    Segment;                                        // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    union                                                                       // 2 / 2 elements; 0x0008 / 0x0010 Bytes
    {
        LIST_ENTRY              ListHead;                                       // 0x0004 / 0x0008; 0x0008 / 0x0010 Bytes
        PVOID                   AweContext;                                     // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
    };
    UINT_PTR                    NumberOfSectionReferences;                      // 0x000C / 0x0018; 0x0004 / 0x0008 Bytes
    UINT_PTR                    NumberOfPfnReferences;                          // 0x0010 / 0x0020; 0x0004 / 0x0008 Bytes
    UINT_PTR                    NumberOfMappedViews;                            // 0x0014 / 0x0028; 0x0004 / 0x0008 Bytes
    UINT_PTR                    NumberOfUserReferences;                         // 0x0018 / 0x0030; 0x0004 / 0x0008 Bytes
    union                                                                       // 0x001C / 0x0038; 2 / 2 elements; 0x0004 / 0x0004 Bytes
    {
        ULONG32                 LongFlags;                                      // 0x001C / 0x0038; 0x0004 / 0x0004 Bytes
        MMSECTION_FLAGS Flags;                                                  // 0x001C / 0x0038; 0x0004 / 0x0004 Bytes
    } u;                                                                        // 0x001C / 0x0038; 0x0004 / 0x0004 Bytes
#if defined(_M_X64)
    union                                                                       // ------ / 0x003C; - / 2 elements; ------ / 0x0004 Bytes
    {
        ULONG32                 LongFlags;                                      // ------ / 0x003C; ------ / 0x0004 Bytes
        MMSECTION_FLAGS2 Flags;                                                 // ------ / 0x003C; ------ / 0x0004 Bytes
    } u1;                                                                       // ------ / 0x003C; ------ / 0x0004 Bytes
#endif                                                                          // #if defined(_M_X64)
    EX_FAST_REF                 FilePointer;                                    // 0x0020 / 0x0040; 0x0004 / 0x0008 Bytes
    LONG32                      ControlAreaLock;                                // 0x0024 / 0x0048; 0x0004 / 0x0004 Bytes
    ULONG32                     ModifiedWriteCount;                             // 0x0028 / 0x004C; 0x0004 / 0x0004 Bytes
    PVOID /*PMI_CONTROL_AREA_WAIT_BLOCK */ 
                                WaitList;                                       // 0x002C / 0x0050; 0x0004 / 0x0008 Bytes
    union                                                                       // 0x0030 / 0x0058; 1 / 1 elements; 0x000C / 0x0010 Bytes
    {
        struct                                                                  // 0x0030 / 0x0058; 15 / 15 elements; 0x000C / 0x0010 Bytes
        {
            union                                                               // 0x0030 / 0x0058; 2 / 2 elements; 0x0004 / 0x0004 Bytes
            {
                ULONG32         NumberOfSystemCacheViews;                       // 0x0000 / 0x0000; 0x0004 / 0x0004 Bytes
                ULONG32         ImageRelocationStartBit;                        // 0x0000 / 0x0000; 0x0004 / 0x0004 Bytes
            };
            union                                                               // 0x0034 / 0x005C; 2 / 2 elements; 0x0004 / 0x0004 Bytes
            {
                LONG32          WritableUserReferences;                         // 0x0004 / 0x0004; 0x0004 / 0x0004 Bytes
                struct                                                          // 0x0004 / 0x0004; 9 / 9 elements; 0x0004 / 0x0004 Bytes
                {
                    ULONG32     ImageRelocationSizeIn64k                   :16; // 0x0004 / 0x0004; Bits:  0 - 15
                    ULONG32     LargePage                                  : 1; // 0x0004 / 0x0004; Bit:  16
                    ULONG32     AweSection                                 : 1; // 0x0004 / 0x0004; Bit:  17
                    ULONG32     SystemImage                                : 1; // 0x0004 / 0x0004; Bit:  18
                    ULONG32     StrongCode                                 : 2; // 0x0004 / 0x0004; Bits: 19 - 20
                    ULONG32     CantMove                                   : 1; // 0x0004 / 0x0004; Bit:  21
                    ULONG32     BitMap                                     : 2; // 0x0004 / 0x0004; Bits: 22 - 23
                    ULONG32     ImageActive                                : 1; // 0x0004 / 0x0004; Bit:  24
                    ULONG32     ImageBaseOkToReuse                         : 1; // 0x0004 / 0x0004; Bit:  25
                };
            };
            union                                                               // 0x0038 / 0x0060; 3 / 3 elements; 0x0004 / 0x0008 Bytes
            {
                ULONG32         FlushInProgressCount;                           // 0x0008 / 0x0008; 0x0004 / 0x0004 Bytes
                ULONG32         NumberOfSubsections;                            // 0x0008 / 0x0008; 0x0004 / 0x0004 Bytes
                PVOID /*PMI_IMAGE_SECURITY_REFERENCE*/
                                SeImageStub;                                    // 0x0008 / 0x0008; 0x0004 / 0x0008 Bytes
            };
        } e2;                                                                   // 0x0030 / 0x0058; 0x000C / 0x0010 Bytes
    } u2;                                                                       // 0x0030 / 0x0058; 0x000C / 0x0010 Bytes
    EX_PUSH_LOCK                FileObjectLock;                                 // 0x003C / 0x0068; 0x0004 / 0x0008 Bytes
    UINT64                      LockedPages;                                    // 0x0040 / 0x0070; 0x0008 / 0x0008 Bytes
#if defined(_M_X64)
    union                                                                       // ------ / 0x0078; - / 4 elements; ------ / 0x0008 Bytes
    {
        UINT64                  IoAttributionContext                       :61; // ------ / 0x0078; Bits:  0 - 60
        UINT64                  Spare                                      : 3; // ------ / 0x0078; Bits: 61 - 63
        UINT64                  ImageCrossPartitionCharge;                      // ------ / 0x0078; ------ / 0x0008 Bytes
        UINT64                  CommittedPageCount                         :36; // ------ / 0x0078; Bits:  0 - 35
    } u3;                                                                       // ------ / 0x0078; ------ / 0x0008 Bytes
#else                                                                           // #if defined(_M_X64)
    union                                                                       // 0x0048 / ------; 4 / - elements; 0x0004 / ------ Bytes
    {
        ULONG32                 IoAttributionContext                       :29; // 0x0048 / ------; Bits:  0 - 28
        ULONG32                 Spare                                      : 3; // 0x0048 / ------; Bits: 29 - 31
        ULONG32                 ImageCrossPartitionCharge;                      // 0x0048 / ------; 0x0004 / ------ Bytes
        ULONG32                 CommittedPageCount                         :20; // 0x0048 / ------; Bits:  0 - 19
    } u3;                                                                       // 0x0048 / ------; 0x0004 / ------ Bytes
#endif                                                                          // #if defined(_M_X64)
#if !defined(_M_X64)
    UINT8                       _PADDING0_[4];                                  // 0x004C / ------; 0x0004 / ------ Bytes
#endif                                                                          // #if !defined(_M_X64)
} CONTROL_AREA, * PCONTROL_AREA;

typedef struct _SUBSECTION                                                      // 14 / 14 elements; 0x0028 / 0x0038 Bytes
{
    PCONTROL_AREA               ControlArea;                                    // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    PMMPTE                      SubsectionBase;                                 // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
    _SUBSECTION*                 NextSubsection;                                 // 0x0008 / 0x0010; 0x0004 / 0x0008 Bytes
    union                                                                       // 3 / 3 elements; 0x0004 / 0x0008 Bytes
    {
        RTL_AVL_TREE            GlobalPerSessionHead;                           // 0x000C / 0x0018; 0x0004 / 0x0008 Bytes
//        PMI_CONTROL_AREA_WAIT_BLOCK
//                                CreationWaitList;                               // 0x000C / 0x0018; 0x0004 / 0x0008 Bytes
//        PMI_PER_SESSION_PROTOS  SessionDriverProtos;                            // 0x000C / 0x0018; 0x0004 / 0x0008 Bytes
    };
    union                                                                       // 0x0010 / 0x0020; 2 / 2 elements; 0x0004 / 0x0004 Bytes
    {
        ULONG32                 LongFlags;                                      // 0x0010 / 0x0020; 0x0004 / 0x0004 Bytes
        MMSUBSECTION_FLAGS SubsectionFlags;                                     // 0x0010 / 0x0020; 0x0004 / 0x0004 Bytes
    } u;                                                                        // 0x0010 / 0x0020; 0x0004 / 0x0004 Bytes
    ULONG32                     StartingSector;                                 // 0x0014 / 0x0024; 0x0004 / 0x0004 Bytes
    ULONG32                     NumberOfFullSectors;                            // 0x0018 / 0x0028; 0x0004 / 0x0004 Bytes
    ULONG32                     PtesInSubsection;                               // 0x001C / 0x002C; 0x0004 / 0x0004 Bytes
    union                                                                       // 0x0020 / 0x0030; 2 / 2 elements; 0x0004 / 0x0004 Bytes
    {
        MI_SUBSECTION_ENTRY1 e1;                                                // 0x0020 / 0x0030; 0x0004 / 0x0004 Bytes
        ULONG32                 EntireField;                                    // 0x0020 / 0x0030; 0x0004 / 0x0004 Bytes
    } u1;                                                                       // 0x0020 / 0x0030; 0x0004 / 0x0004 Bytes
    struct                                                                      // 3 / 3 elements; 0x0004 / 0x0004 Bytes
    {
        ULONG32                 UnusedPtes                                 :30; // 0x0024 / 0x0034; Bits:  0 - 29
        ULONG32                 ExtentQueryNeeded                          : 1; // 0x0024 / 0x0034; Bit:  30
        ULONG32                 DirtyPages                                 : 1; // 0x0024 / 0x0034; Bit:  31
    };
} SUBSECTION, * PSUBSECTION;


typedef struct _MI_VAD_SEQUENTIAL_INFO                                          // 2 / 2 elements; 0x0004 / 0x0008 Bytes
{
#if defined(_M_X64)
    UINT64                      Length                                     :12; // ------ / 0x0000; Bits:  0 - 11
#else                                                                           // #if defined(_M_X64)
    ULONG32                     Length                                     :11; // 0x0000 / ------; Bits:  0 - 10
#endif                                                                          // #if defined(_M_X64)
#if defined(_M_X64)
    UINT64                      Vpn                                        :52; // ------ / 0x0000; Bits: 12 - 63
#else                                                                           // #if defined(_M_X64)
    ULONG32                     Vpn                                        :21; // 0x0000 / ------; Bits: 11 - 31
#endif                                                                          // #if defined(_M_X64)
} MI_VAD_SEQUENTIAL_INFO, * PMI_VAD_SEQUENTIAL_INFO;

typedef struct _SECTION_OBJECT_POINTERS                                         // 3 / 3 elements; 0x000C / 0x0018 Bytes
{
    PVOID                       DataSectionObject;                              // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    PVOID                       SharedCacheMap;                                 // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
    PVOID                       ImageSectionObject;                             // 0x0008 / 0x0010; 0x0004 / 0x0008 Bytes
} SECTION_OBJECT_POINTERS, * PSECTION_OBJECT_POINTERS;

typedef struct _UNICODE_STRING                                                  // 3 / 4 elements; 0x0008 / 0x0010 Bytes
{
    UINT16                      Length;                                         // 0x0000 / 0x0000; 0x0002 / 0x0002 Bytes
    UINT16                      MaximumLength;                                  // 0x0002 / 0x0002; 0x0002 / 0x0002 Bytes
#if defined(_M_X64)
    UINT8                       _PADDING0_[4];                                  // ------ / 0x0004; ------ / 0x0004 Bytes
#endif                                                                          // #if defined(_M_X64)
    WCHAR *                     Buffer;                                         // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
} UNICODE_STRING, * PUNICODE_STRING;

typedef union _LARGE_INTEGER                                                    // 4 / 4 elements; 0x0008 / 0x0008 Bytes
{
    ULONG32                     LowPart;                                        // 0x0000 / 0x0000; 0x0004 / 0x0004 Bytes
    LONG32                      HighPart;                                       // 0x0004 / 0x0004; 0x0004 / 0x0004 Bytes
    struct                                                                      // 2 / 2 elements; 0x0008 / 0x0008 Bytes
    {
        ULONG32                 LowPart;                                        // 0x0000 / 0x0000; 0x0004 / 0x0004 Bytes
        LONG32                  HighPart;                                       // 0x0004 / 0x0004; 0x0004 / 0x0004 Bytes
    } u;                                                                        // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
    INT64                       QuadPart;                                       // 0x0000 / 0x0000; 0x0008 / 0x0008 Bytes
} LARGE_INTEGER, * PLARGE_INTEGER;

typedef struct _KEVENT                                                          // 1 / 1 elements; 0x0010 / 0x0018 Bytes
{
	UINT8				_PADDING0_[0x18];
//    DISPATCHER_HEADER           Header;                                         // 0x0000 / 0x0000; 0x0010 / 0x0018 Bytes
} KEVENT, * PKEVENT;

typedef struct _IO_COMPLETION_CONTEXT                                           // 2 / 2 elements; 0x0008 / 0x0010 Bytes
{
    PVOID                       Port;                                           // 0x0000 / 0x0000; 0x0004 / 0x0008 Bytes
    PVOID                       Key;                                            // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
} IO_COMPLETION_CONTEXT, * PIO_COMPLETION_CONTEXT;

typedef struct _FILE_OBJECT                                                     // 30 / 33 elements; 0x0080 / 0x00D8 Bytes
{
    INT16                       Type;                                           // 0x0000 / 0x0000; 0x0002 / 0x0002 Bytes
    INT16                       Size;                                           // 0x0002 / 0x0002; 0x0002 / 0x0002 Bytes
#if defined(_M_X64)
    UINT8                       _PADDING0_[4];                                  // ------ / 0x0004; ------ / 0x0004 Bytes
#endif                                                                          // #if defined(_M_X64)
    PVOID /*PDEVICE_OBJECT*/    DeviceObject;                                   // 0x0004 / 0x0008; 0x0004 / 0x0008 Bytes
    PVOID /*PVDB*/              Vpb;                                            // 0x0008 / 0x0010; 0x0004 / 0x0008 Bytes
    PVOID                       FsContext;                                      // 0x000C / 0x0018; 0x0004 / 0x0008 Bytes
    PVOID                       FsContext2;                                     // 0x0010 / 0x0020; 0x0004 / 0x0008 Bytes
    PSECTION_OBJECT_POINTERS    SectionObjectPointer;                           // 0x0014 / 0x0028; 0x0004 / 0x0008 Bytes
    PVOID                       PrivateCacheMap;                                // 0x0018 / 0x0030; 0x0004 / 0x0008 Bytes
    LONG32                      FinalStatus;                                    // 0x001C / 0x0038; 0x0004 / 0x0004 Bytes
#if defined(_M_X64)
    UINT8                       _PADDING1_[4];                                  // ------ / 0x003C; ------ / 0x0004 Bytes
#endif                                                                          // #if defined(_M_X64)
    _FILE_OBJECT*               RelatedFileObject;                              // 0x0020 / 0x0040; 0x0004 / 0x0008 Bytes
    UINT8                       LockOperation;                                  // 0x0024 / 0x0048; 0x0001 / 0x0001 Bytes
    UINT8                       DeletePending;                                  // 0x0025 / 0x0049; 0x0001 / 0x0001 Bytes
    UINT8                       ReadAccess;                                     // 0x0026 / 0x004A; 0x0001 / 0x0001 Bytes
    UINT8                       WriteAccess;                                    // 0x0027 / 0x004B; 0x0001 / 0x0001 Bytes
    UINT8                       DeleteAccess;                                   // 0x0028 / 0x004C; 0x0001 / 0x0001 Bytes
    UINT8                       SharedRead;                                     // 0x0029 / 0x004D; 0x0001 / 0x0001 Bytes
    UINT8                       SharedWrite;                                    // 0x002A / 0x004E; 0x0001 / 0x0001 Bytes
    UINT8                       SharedDelete;                                   // 0x002B / 0x004F; 0x0001 / 0x0001 Bytes
    ULONG32                     Flags;                                          // 0x002C / 0x0050; 0x0004 / 0x0004 Bytes
#if defined(_M_X64)
    UINT8                       _PADDING2_[4];                                  // ------ / 0x0054; ------ / 0x0004 Bytes
#endif                                                                          // #if defined(_M_X64)
    UNICODE_STRING              FileName;                                       // 0x0030 / 0x0058; 0x0008 / 0x0010 Bytes
    LARGE_INTEGER               CurrentByteOffset;                              // 0x0038 / 0x0068; 0x0008 / 0x0008 Bytes
    ULONG32                     Waiters;                                        // 0x0040 / 0x0070; 0x0004 / 0x0004 Bytes
    ULONG32                     Busy;                                           // 0x0044 / 0x0074; 0x0004 / 0x0004 Bytes
    PVOID                       LastLock;                                       // 0x0048 / 0x0078; 0x0004 / 0x0008 Bytes
    KEVENT                      Lock;                                           // 0x004C / 0x0080; 0x0010 / 0x0018 Bytes
    KEVENT                      Event;                                          // 0x005C / 0x0098; 0x0010 / 0x0018 Bytes
    PIO_COMPLETION_CONTEXT      CompletionContext;                              // 0x006C / 0x00B0; 0x0004 / 0x0008 Bytes
    UINT_PTR                    IrpListLock;                                    // 0x0070 / 0x00B8; 0x0004 / 0x0008 Bytes
    LIST_ENTRY                  IrpList;                                        // 0x0074 / 0x00C0; 0x0008 / 0x0010 Bytes
    PVOID                       FileObjectExtension;                            // 0x007C / 0x00D0; 0x0004 / 0x0008 Bytes
} FILE_OBJECT, * PFILE_OBJECT;

typedef struct _MMVAD                                                           //  9 / 10 elements; 0x004C / 0x0088 Bytes
{
    MMVAD_SHORT                 Core;                                           // 0x0000 / 0x0000; 0x0028 / 0x0040 Bytes
    union                                                                       // 0x0028 / 0x0040; 2 / 2 elements; 0x0004 / 0x0004 Bytes
    {
        ULONG32                 LongFlags2;                                     // 0x0028 / 0x0040; 0x0004 / 0x0004 Bytes
        MMVAD_FLAGS2 VadFlags2;                                                 // 0x0028 / 0x0040; 0x0004 / 0x0004 Bytes
    } u2;                                                                       // 0x0028 / 0x0040; 0x0004 / 0x0004 Bytes
#if defined(_M_X64)
    UINT8                       _PADDING0_[4];                                  // ------ / 0x0044; ------ / 0x0004 Bytes
#endif                                                                          // #if defined(_M_X64)
    PSUBSECTION                 Subsection;                                     // 0x002C / 0x0048; 0x0004 / 0x0008 Bytes
    PMMPTE                      FirstPrototypePte;                              // 0x0030 / 0x0050; 0x0004 / 0x0008 Bytes
    PMMPTE                      LastContiguousPte;                              // 0x0034 / 0x0058; 0x0004 / 0x0008 Bytes
    LIST_ENTRY                  ViewLinks;                                      // 0x0038 / 0x0060; 0x0008 / 0x0010 Bytes
    PVOID /* PEPROCESS */       VadsProcess;                                    // 0x0040 / 0x0070; 0x0004 / 0x0008 Bytes
    union                                                                       // 0x0044 / 0x0078; 2 / 2 elements; 0x0004 / 0x0008 Bytes
    {
        MI_VAD_SEQUENTIAL_INFO SequentialVa;                                    // 0x0044 / 0x0078; 0x0004 / 0x0008 Bytes
        PMMEXTEND_INFO          ExtendedInfo;                                   // 0x0044 / 0x0078; 0x0004 / 0x0008 Bytes
    } u4;                                                                       // 0x0044 / 0x0078; 0x0004 / 0x0008 Bytes
    PFILE_OBJECT                FileObject;                                     // 0x0048 / 0x0080; 0x0004 / 0x0008 Bytes
} MMVAD, * PMMVAD;





#pragma pack(pop)
} // namespace tim
#endif // TIM_WINSTRUCTS_H
