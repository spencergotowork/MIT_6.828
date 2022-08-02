#include <kern/e1000.h>
#include <kern/pmap.h>

// LAB 6: Your driver code here
#define E1000REG(offset) (void*)(bar_va + offset)
volatile void *bar_va;

int e1000_attachfunc(struct pci_func *pcif) {
    pci_func_enable(pcif);
    bar_va = mmio_map_region(pcif->reg_base[0], pcif->reg_size[0]);

    uint32_t *status_reg = E1000REG(E1000_STATUS);
    assert(*status_reg == 0x80080783);
    return 0;
}

