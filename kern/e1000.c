#include <kern/e1000.h>
#include <kern/pmap.h>
#include <inc/string.h>

// LAB 6: Your driver code here
#define E1000REG(offset) (void*)(bar_va + offset)
volatile void *bar_va;

// define tdh tdt tdlen
struct e1000_tdh *tdh;
struct e1000_tdt *tdt;
struct e1000_tdlen *tdlen;

struct e1000_tx_desc tx_desc_list[TXDESCS];
char tx_buffer_array[TXDESCS][TX_PKT_SIZE];

int e1000_attachfunc(struct pci_func *pcif) {
    pci_func_enable(pcif);
    bar_va = mmio_map_region(pcif->reg_base[0], pcif->reg_size[0]);

    uint32_t *status_reg = E1000REG(E1000_STATUS);
    cprintf("e1000: status 0x%08x\n", E1000REG(E1000_STATUS));

    assert(*status_reg == 0x80080783);
    e1000_transmit_init();
    return 0;
}

static void
e1000_transmit_init() {
    	int i;
	for (i = 0; i < TXDESCS; i++) {
		tx_desc_list[i].addr = PADDR(tx_buffer_array[i]);
		tx_desc_list[i].cmd = 0;
		tx_desc_list[i].status |= E1000_TXD_STAT_DD;
	}

	struct e1000_tdlen *tdlen = (struct e1000_tdlen *)E1000REG(E1000_TDLEN);
	tdlen->len = TXDESCS;

	uint32_t *tdbal = (uint32_t *)E1000REG(E1000_TDBAL);
	*tdbal = PADDR(tx_desc_list);

	uint32_t *tdbah = (uint32_t *)E1000REG(E1000_TDBAH);
	*tdbah = 0;

	tdh = (struct e1000_tdh *)E1000REG(E1000_TDH);
	tdh->tdh = 0;

	tdt = (struct e1000_tdt *)E1000REG(E1000_TDT);
	tdt->tdt = 0;

	struct e1000_tctl *tctl = (struct e1000_tctl *)E1000REG(E1000_TCTL);
	tctl->en = 1;
	tctl->psp = 1;
	tctl->ct = 0x10;
	tctl->cold = 0x40;

	struct e1000_tipg *tipg = (struct e1000_tipg *)E1000REG(E1000_TIPG);
	tipg->ipgt = 10;
	tipg->ipgr1 = 4;
	tipg->ipgr2 = 6;
    
}

int e1000_transmit(void *data, size_t len)
{
    uint32_t current = tdt->tdt;
    if(!(tx_desc_list[current].status & E1000_TXD_STAT_DD)) {
        return E_TRANSMIT_RETRY;
    }
    tx_desc_list[current].length = len;
    tx_desc_list[current].status &= ~E1000_TXD_STAT_DD;
    tx_desc_list[current].cmd |= (E1000_TXD_CMD_EOP | E1000_TXD_CMD_RS);

    //memcpy(void *dst, const void *src, size_t n)
    memcpy(tx_buffer_array, data, len);
    uint32_t next = (current + 1) % TXDESCS;
    tdt->tdt = next;
    return 0;
}

