#include "../headers/pgwalk.h"
#include "../headers/log.h"
#include "../headers/export.h"

void judge(unsigned long pte_flags, int flag) {
    if (flag == 0) {
        // code page trampoline.
        // vs_err("pte_flags: %lx %lx %lx", pte_flags, pte_flags >> 51, pte_flags >> 52);
        if ((pte_flags >> 51) == 1 && (pte_flags >> 52) == 0) {
            vs_err("code page trampoline succeed.\n");
        } else {
            vs_err("code page trampoline fail.\n");
        }
    } else {
        // data page trampoline.
        // vs_err("pte_flags: %lx %lx %lx", pte_flags, (pte_flags >> 63), (pte_flags >> 62) & 1);
        if ((pte_flags >> 63) == 1 && ((pte_flags >> 62) & 1) == 0) {
            vs_err("data page trampoline succeed.\n");
        } else {
            vs_err("data page trampoline fail.\n");
        }
    }
}

// when the pagetable already has the entry.
void print_page_table_entry(unsigned long address, int flag) {
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;

    pgd = pgd_offset(current->mm, address);
    vs_err("belonging of pagetable walk: %s", current->comm);

    if (pgd_none(*pgd)) {
        vs_err("Invalid PGD entry\n");
        return;
    }

    p4d = p4d_offset(pgd, address);
    if (p4d_none(*p4d)) {
        vs_err("Invalid P4D entry\n");
        return;
    }

    pud = pud_offset(p4d, address);

    if (pud_none(*pud)) {
        vs_err("Invalid PUD entry\n");
        return;
    }

    pmd = pmd_offset(pud, address);

    if (pmd_none(*pmd)) {
        vs_err("Invalid PMD entry\n");
        return;
    }

    pte = pte_offset_kernel(pmd, address);

    if (pte_present(*pte)) {
        unsigned long pte_flags = pte_val(*pte);
        vs_err("Page table entry at virtual address %lx:\n", address);
        vs_err("  PTE Value: %lx\n", pte_flags);
        judge(pte_flags, flag);
    } else {
        vs_err("Page not present at virtual address %lx\n", address);
        vs_err("trampoline initialization failed");
    }
    pte_unmap(pte); 
}

// if the pagetable already has the entry, clear the NX bit.
void set_pte_x(unsigned long address) {
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t* ptep;
    pte_t new_pte;
    vs_err("belonging of pagetable walk: %s", current->comm);
    pgd = pgd_offset(current->mm, address);

    if (pgd_none(*pgd)) {
        vs_err("Invalid PGD entry\n");
        return;
    }

    p4d = p4d_offset(pgd, address);
    if (p4d_none(*p4d)) {
        vs_err("Invalid P4D entry\n");
        return;
    }

    pud = pud_offset(p4d, address);

    if (pud_none(*pud)) {
        vs_err("Invalid PUD entry\n");
        return;
    }

    pmd = pmd_offset(pud, address);

    if (pmd_none(*pmd)) {
        vs_err("Invalid PMD entry\n");
        return;
    }

    ptep = pte_offset_kernel(pmd, address);
    if (pte_present(*ptep)){
        unsigned long pte_flags = pte_val(*ptep);
        vs_err("Page table entry at virtual address %lx:\n", address);
        new_pte = pte_clear_flags(*ptep, _PAGE_NX);
        unsigned long new_pte_flags = pte_val(new_pte);
        vs_err("  PTE New Value: %lx\n", new_pte_flags);
        set_pte(ptep, new_pte);
        
        __asm__ __volatile__ (".byte 0x0f,0x01,0xfe" : : "a"(0xf), "c"(0), "d"(0) : );
        __asm__ __volatile__ (".byte 0x0f,0x01,0xff" : : : );
        vs_err("tlb flush is done");
    }
    pte_unmap(ptep); 
}

// if the pagetable already has the entry, clear the NX bit.
void set_pte_pkru(unsigned long address, int key) {
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t* ptep;
    pte_t new_pte;
    vs_err("belonging of pagetable walk: %s", current->comm);
    pgd = pgd_offset(current->mm, address);

    if (pgd_none(*pgd)) {
        vs_err("Invalid PGD entry\n");
        return;
    }

    p4d = p4d_offset(pgd, address);
    if (p4d_none(*p4d)) {
        vs_err("Invalid P4D entry\n");
        return;
    }

    pud = pud_offset(p4d, address);

    if (pud_none(*pud)) {
        vs_err("Invalid PUD entry\n");
        return;
    }

    pmd = pmd_offset(pud, address);

    if (pmd_none(*pmd)) {
        vs_err("Invalid PMD entry\n");
        return;
    }

    ptep = pte_offset_kernel(pmd, address);
    if (pte_present(*ptep)){
        unsigned long pte_flags = pte_val(*ptep);
        vs_err("Page table entry at virtual address %lx:\n", address);
        new_pte = pte_clear_flags(*ptep, _PAGE_NX);
        unsigned long new_pte_flags = pte_val(new_pte);
        vs_err("  PTE New Value: %lx\n", new_pte_flags);
        set_pte(ptep, new_pte);
        
        __asm__ __volatile__ (".byte 0x0f,0x01,0xfe" : : "a"(0xf), "c"(0), "d"(0) : );
        __asm__ __volatile__ (".byte 0x0f,0x01,0xff" : : : );
        vs_err("tlb flush is done");
    }
    pte_unmap(ptep); 
}

// isn't necessary to have pgd, p4d, pud, pmd, pte entries for user application.
// so basically we should add the entries during pagetable walk process.
void app_pgwalk(unsigned long virt_addr, unsigned long phys_addr, int not_execute_flag) {
    // vs_err("virt_addr: %lx phys_addr: %lx", virt_addr, phys_addr);
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t* ptep;
    pte_t new_pte;

    // vs_err("belonging of pagetable walk: %s", current->comm);
    struct mm_struct* current_mm = current->mm;
    pgd = pgd_offset(current_mm, virt_addr);

    if (pgd_none(*pgd)) {
        // vs_err("PGD entry not found, creating...\n");
        __p4d_alloc_sym(current_mm, pgd, virt_addr);
        p4d = p4d_offset(pgd, virt_addr);       
        if (!p4d) {
            // vs_err("Failed to allocate P4D\n");
            return;
        }
    } else {
        p4d = p4d_offset(pgd, virt_addr);
    }

    if (p4d_none(*p4d)) {
        // vs_err("P4D entry not found, creating...\n");
        __pud_alloc_sym(current_mm, p4d, virt_addr);
        pud = pud_offset(p4d, virt_addr);
        if (!p4d) {
            // vs_err("Failed to allocate PUD\n");
            return;
        }
    } else {
        pud = pud_offset(p4d, virt_addr);
    }

    if (pud_none(*pud)) {
        // vs_err("PUD entry not found, creating...\n");
        __pmd_alloc_sym(current_mm, pud, virt_addr);
        pmd = pmd_offset(pud, virt_addr);
        if (!p4d) {
            // vs_err("Failed to allocate PMD\n");
            return;
        }
    } else {
        pmd = pmd_offset(pud, virt_addr);
    }

    if (pmd_none(*pmd)) {
        // vs_err("PMD entry not found, creating...\n");
        __pte_alloc_sym(current_mm, pmd);
        ptep = pte_offset_kernel(pmd, virt_addr);
        if (!ptep) {
            // vs_err("Failed to allocate PTE\n");
            return;
        }
    } else {
        ptep = pte_offset_kernel(pmd, virt_addr);
    }

    unsigned long phys_frame = phys_addr >> PAGE_SHIFT;
    // vs_err("phys_addr: %lx phys_frame: %lx", phys_addr, phys_frame);
    // it's quite confusing that pfn_pte delete the higher bit of the pfn.
    // so we simply use a more dirty way to set pte here.
    unsigned long new_pte_val = (phys_frame << PAGE_SHIFT) | 0x163;
    if (not_execute_flag == 1) {
        new_pte_val |= 0x8000000000000000 | new_pte_val;
    }
    // vs_err("new_pte_val: %lx", new_pte_val);
    new_pte = __pte(new_pte_val);
    unsigned long new_pte_flags = pte_val(new_pte);

    // vs_err("  PTE New Value: %lx", new_pte_flags);
    set_pte(ptep, new_pte);

    __asm__ __volatile__ (".byte 0x0f,0x01,0xfe" : : "a"(0xf), "c"(0), "d"(0) : );
    __asm__ __volatile__ (".byte 0x0f,0x01,0xff" : : : );

    // vs_err("tlb flush is done");

    // vs_err("Execute bit after:  %d", pte_exec(*ptep));

    pte_unmap(ptep);
}