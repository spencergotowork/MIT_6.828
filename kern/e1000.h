#include <kern/pci.h>

#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#define E1000_VENDOR_ID 0x8086
#define E1000_DEVICE_ID 0x100E

#define E1000_STATUS 0x00008 // Device status
#define E1000_TDBAL 0x03800
#define E1000_TDBAH 0x03804
#define E1000_TDLEN 0x03808
#define E1000_TDH 0x03810
#define E1000_TDT 0x03818
#define E1000_TCTL 0x00400
#define E1000_TIPG 0x00410
#define TXDESCS 32
#define TX_PKT_SIZE 1518
#define E1000_TXD_STAT_DD    0x00000001 /* Descriptor Done */
#define E1000_TXD_CMD_EOP    0x00000001 /* End of Packet */
#define E1000_TXD_CMD_RS     0x00000008 /* Report Status */

// #define E1000_
// #define E1000_

enum {
	E_TRANSMIT_RETRY = -1,
	E_RECEIVE_RETRY
};

struct e1000_tx_desc // copy from handbook web
{
	uint64_t addr;
	uint16_t length;
	uint8_t cso;
	uint8_t cmd;
	uint8_t status;
	uint8_t css;
	uint16_t special;
}__attribute__((packed));

struct e1000_tdlen
{
	/* data */
	uint32_t zero: 7;
	uint32_t len: 13;
	uint32_t rsv: 12;
};

struct e1000_tdh
{
	/* data */
	uint32_t tdh: 16;
	uint32_t rsv: 16;
};

struct e1000_tdt
{
	/* data */
	uint32_t tdt: 16;
	uint32_t rsv: 16;
};

struct e1000_tidv
{
	/* data */
	uint32_t idv: 16;
	uint32_t rsv: 16;
};

struct e1000_txdmac
{
	/* data */
	uint32_t dpp: 1;
	uint32_t rsv: 31;
};

struct e1000_rxdctl
{
	/* data */
	
};

struct e1000_tctl 
{
       uint32_t rsv1:   1;
       uint32_t en:     1;
       uint32_t rsv2:   1;
       uint32_t psp:    1;
       uint32_t ct:     8;
       uint32_t cold:   10;
       uint32_t swxoff: 1;
       uint32_t rsv3:   1;
       uint32_t rtlc:   1;
       uint32_t nrtu:   1;
       uint32_t rsv4:   6;
};

struct e1000_tipg
{
	/* data */
	uint32_t ipgt:    10;
	uint32_t ipgr1:   10;
	uint32_t ipgr2:   10;
	uint32_t rsv: 2;
};


int  e1000_attachfunc(struct pci_func *pcif);
static void e1000_transmit_init();
int e1000_transmit(void *data, size_t len);

#endif  // SOL >= 6
