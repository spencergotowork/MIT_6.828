// #include <kern/e1000.h>
// #include <kern/pmap.h>
// #include <inc/string.h>

// // LAB 6: Your driver code here
// #define E1000REG(offset) (void*)(bar_va + offset)
// volatile void *bar_va;

// // define tdh tdt tdlen
// struct e1000_tdh *tdh;
// struct e1000_tdt *tdt;
// struct e1000_tdlen *tdlen;

// struct e1000_tx_desc tx_desc_list[TXDESCS];
// char tx_buffer_array[TXDESCS][TX_PKT_SIZE];

// int e1000_attachfunc(struct pci_func *pcif) {
//     pci_func_enable(pcif);
//     bar_va = mmio_map_region(pcif->reg_base[0], pcif->reg_size[0]);

//     uint32_t *status_reg = E1000REG(E1000_STATUS);
//     assert(*status_reg == 0x80080783);
//     e1000_transmit_init();
//     return 0;
// }

// static void
// e1000_transmit_init() {
//     for(int i = 0;i<TXDESCS;i++) {
//         tx_desc_list[i].addr = PADDR(tx_buffer_array[i]);
//         tx_desc_list[i].cmd = 0;
//         tx_desc_list[i].status |= E1000_TXD_STAT_DD;
//     }

//     tdlen = (struct e1000_tdlen*)E1000REG(E1000_TDLEN);
//     tdlen->len = TXDESCS;

//     uint32_t *tdbal = (uint32_t*)E1000REG(E1000_TDBAL);
//     *tdbal = PADDR(tx_desc_list);

//     uint32_t *tdbah = (uint32_t*)E1000REG(E1000_TDBAH);
//     *tdbah = 0;

//     tdh = (struct e1000_tdh*)E1000REG(E1000_TDH);
//     tdh->tdh = 0;

//     tdt = (struct e1000_tdt*)E1000REG(E1000_TDT);
//     tdt->tdt = 0;

//     struct e1000_tctl *tctl = (struct e1000_tctl*)E1000REG(E1000_TCTL);
//     tctl->en = 1;
//     tctl->psp = 1;
//     tctl->ct = 0x10;
//     tctl->cold = 0x40;
    
//     struct e1000_tipg *tipg = (struct e1000_tipg*)E1000REG(E1000_TIPG);
//     tipg->ipgt = 10;
//     tipg->ipgr1 = 4;
//     tipg->ipgr2 = 6;
    
// }

// int e1000_transmit(void *data, size_t len)
// {
//     uint32_t current = tdt->tdt;
//     if(!(tx_desc_list[current].status & E1000_TXD_STAT_DD)) {
//         return E_TRANSMIT_RETRY;
//     }
//     tx_desc_list[current].length = len;
//     tx_desc_list[current].status &= ~E1000_TXD_STAT_DD;
//     tx_desc_list[current].cmd |= (E1000_TXD_CMD_EOP | E1000_TXD_CMD_RS);

//     //memcpy(void *dst, const void *src, size_t n)
//     memcpy(tx_buffer_array, data, len);
//     uint32_t next = (current + 1) % TXDESCS;
//     tdt->tdt = next;
//     return 0;
// }

#include <kern/e1000.h>
#include <kern/pmap.h>
#include <inc/string.h>

// LAB 6: Your driver code here

volatile void *bar_va;
struct e1000_tdh *tdh;
struct e1000_tdt *tdt;
struct e1000_tx_desc tx_desc_array[TXDESCS];
char tx_buffer_array[TXDESCS][TX_PKT_SIZE];
#define E1000REG(offset) (void*)(bar_va + offset)

uint32_t E1000_MAC[6] = {0x52, 0x54, 0x00, 0x12, 0x34, 0x56};

int
e1000_attachfunc(struct pci_func *pcif)
{
	pci_func_enable(pcif);
	cprintf("reg_base:%x, reg_size:%x\n", pcif->reg_base[0], pcif->reg_size[0]);
			
	//Exercise4 create virtual memory mapping
	bar_va = mmio_map_region(pcif->reg_base[0], pcif->reg_size[0]);
	
	uint32_t *status_reg = E1000REG(E1000_STATUS);
	assert(*status_reg == 0x80080783);

	e1000_transmit_init();

	return 0;
}

//see section 14.5 in https://pdos.csail.mit.edu/6.828/2018/labs/lab6/e1000_hw.h
static void
e1000_transmit_init()
{
       int i;
       for (i = 0; i < TXDESCS; i++) {
               tx_desc_array[i].addr = PADDR(tx_buffer_array[i]);
               tx_desc_array[i].cmd = 0;
               tx_desc_array[i].status |= E1000_TXD_STAT_DD;
       }
			 //TDLEN register
       struct e1000_tdlen *tdlen = (struct e1000_tdlen *)E1000REG(E1000_TDLEN);
       tdlen->len = TXDESCS;
			 
			 //TDBAL register
       uint32_t *tdbal = (uint32_t *)E1000REG(E1000_TDBAL);
       *tdbal = PADDR(tx_desc_array);

			 //TDBAH regsiter
       uint32_t *tdbah = (uint32_t *)E1000REG(E1000_TDBAH);
       *tdbah = 0;

		   //TDH register, should be init 0
       tdh = (struct e1000_tdh *)E1000REG(E1000_TDH);
       tdh->tdh = 0;

		   //TDT register, should be init 0
       tdt = (struct e1000_tdt *)E1000REG(E1000_TDT);
       tdt->tdt = 0;

			 //TCTL register
       struct e1000_tctl *tctl = (struct e1000_tctl *)E1000REG(E1000_TCTL);
       tctl->en = 1;
       tctl->psp = 1;
       tctl->ct = 0x10;
       tctl->cold = 0x40;

			 //TIPG register
       struct e1000_tipg *tipg = (struct e1000_tipg *)E1000REG(E1000_TIPG);
       tipg->ipgt = 10;
       tipg->ipgr1 = 4;
       tipg->ipgr2 = 6;
}


int
e1000_transmit(void *data, size_t len)
{
       uint32_t current = tdt->tdt;		//tail index in queue
       if(!(tx_desc_array[current].status & E1000_TXD_STAT_DD)) {
               return -E_TRANSMIT_RETRY;
       }
       tx_desc_array[current].length = len;
       tx_desc_array[current].status &= ~E1000_TXD_STAT_DD;
       tx_desc_array[current].cmd |= (E1000_TXD_CMD_EOP | E1000_TXD_CMD_RS);
       memcpy(tx_buffer_array[current], data, len);
       uint32_t next = (current + 1) % TXDESCS;
       tdt->tdt = next;
       return 0;
}

